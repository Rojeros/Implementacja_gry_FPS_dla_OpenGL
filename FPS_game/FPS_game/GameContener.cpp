#include "GameContener.h"


Uint32 GameContener::lastTiks;
GLuint GameContener::TextureID;


GameContener::GameContener() :full_screen(0), screen_width(512), screen_height(512), gameRunning(true), gamePause(false),lvl(1)
{
	map = new Map();
}

int GameContener::Run()
{
	if (!SetupRC())
		return -1;

	StartEngine();
	fpsinit();
	while (gameRunning)
	{
		while (!gamePause) {
			WaitFrame(60);
			ProcessEvents();
			DoEngine();
			Render();

		}
		if (gameRunning) {
			WaitFrame(5);
			RenderPause();
			ProcessEvents();
		}

	}

}


GameContener::~GameContener()
{

	delete  player;
	delete animations;
	delete text;
	delete map;
	delete mapElements;
	delete bullets;
	while (!enemy->empty()) delete enemy->front(), enemy->pop_front();
	delete enemy;

}

bool GameContener::SetupRC()
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}
	Uint32 flags;
	if (full_screen == 1) {
		flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
	}
	else {
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	}
	// Create our window 
	//TODO: zmieniæ na koniec 800 na sdl_windowpos_centered
	mainWindow = SDL_CreateWindow("FPS Game", 800, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, flags);


	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
	// Check that everything worked out okay
	if (!mainWindow)
	{
		std::cout << "Unable to create window\n";
		CheckSDLError(__LINE__);
		return false;
	}

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);

	SetOpenGLAttributes();

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	SDL_ShowCursor(SDL_DISABLE);


	glShadeModel(GL_SMOOTH);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0.25f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, screen_width, screen_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	glEnable(GL_LIGHTING);

	// w³¹czenie œwiat³a GL_LIGHT0 z parametrami domyœlnymi
	glEnable(GL_LIGHT0);
	//defining global ambient
	GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	// Create light components
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -1.5f, 4.0f, 4.0f, 1.0f };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	glMateriali(GL_FRONT, GL_SHININESS, 96);


	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	TTF_Init();
	text = new GameUI(renderer, screen_width, screen_height, 32);

	srand(time(NULL));

	return true;
}

void GameContener::Render()
{
	if (gamePause || !gameRunning)
		return;
	/* rysujemy tutaj */
	glClearColor(0.4, 0.7, 1.0, 0.5); //clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//	glDisable(GL_LIGHTING);
	gluPerspective(45, screen_width / screen_height, 0.1, 500.0);
	//glBegin(GL_LINES);
	//for (int i = 0; i <= 100; i++) {
	//	glColor3f(1, 0, 0);
	//	glVertex3f(100, 0, i);
	//	glVertex3f(-100, 0, i);
	//	glVertex3f(i, 0, 100);
	//	glVertex3f(i, 0, -100);
	//};
	//glEnd();

	fpsthink();




	Uint32 delta = 0;
	Uint32 tmp = SDL_GetTicks();
	delta = tmp - lastTiks;
	lastTiks = tmp;
	float dt = ((float)delta) / 1000.f;

	glDepthFunc(GL_LESS);
	player->show(dt);

	//std::cout<<player->getCamera()->getLocation();

	for (std::list<Enemy*>::iterator it = enemy->begin(); it != enemy->end(); it++) {

		(*it)->show(dt);

	}
	bullets->update(dt, player, enemy, map);

	bullets->draw();
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	mapElements->draw(dt);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	map->renderTerrain();

	//always display gun
	if (!player->isDead()) {
		glClear(GL_DEPTH_BUFFER_BIT);
		if (player->haveAnyGun())
			player->getCurrentWeapon()->show(player->getCamera()->getYaw(), player->getCamera()->getPitch(), dt);
	}

	//	std::cout << newpos << "\n";

		//static float move=0;
		//move += 0.02;
		//glTranslatef(0, 0, move);
		//enemy->draw(dt);

		//glTranslatef(30, 0, 0);
		//enemy[frame]->draw(materials, materialsVertex);
		//glTranslatef(30, 0, 0);
		//enemy[frame]->draw(materials, materialsVertex);
		//glTranslatef(30, 0, 0);
		//enemy[frame]->draw(materials, materialsVertex);
		//glTranslatef(30, 0, 0);
		//enemy[frame]->draw(materials, materialsVertex);
		//glTranslatef(30, 0, 0);
		//enemy[frame]->draw(materials, materialsVertex);


	if (player->isDead()) {

		text->renderText("GAME OVER", { 255,0,0 }, coordinates::CENTER, 0, 0);
		if (keys[MOUSE_LEFT_BUTTION] == 1)
			player->resetPlayer();
	}

	if (player->wasHit(dt)) {
		text->displayRedSquare();
	}

	text->draw();
	//	std::cout << framespersecond << "\n";




	SDL_GL_SwapWindow(mainWindow);



	return;
}
bool GameContener::SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


	return true;
}
void GameContener::HandleKeyDown(SDL_Keysym* keysym)
{
	switch (keysym->scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		QuitGame();
		break;
	default:
		keys[keysym->scancode] = 1;
		break;

	}

	return;
}
void GameContener::HandleKeyUp(SDL_Keysym* keysym)
{
	keys[keysym->scancode] = 0;
	keysChange[keysym->scancode] = 0;

	return;
}
void GameContener::MouseMotion(SDL_MouseMotionEvent * motion)
{
	if (!gamePause) {
		//std::cout<<motion->xrel<<"\n";
		float dx = motion->x - screen_width / 2;
		float dy = motion->y - screen_height / 2;
		SDL_WarpMouseInWindow(mainWindow, (screen_width / 2), (screen_height / 2));
		player->lookAt(dx, dy);
		//int dy = motion->y - screen_height / 2;
	}
}
void GameContener::MouseClickDown(SDL_MouseButtonEvent * click)
{

	if (click[0].type == SDL_MOUSEBUTTONDOWN)
		keys[MOUSE_LEFT_BUTTION] = 1;
	if (click[1].type == SDL_MOUSEBUTTONDOWN)
		keys[MOUSE_MIDDLE_BUTTION] = 1;
	if (click[2].type == SDL_MOUSEBUTTONDOWN)
		keys[MOUSE_RIGHT_BUTTION] = 1;


}

void GameContener::MouseClickUp(SDL_MouseButtonEvent * click)
{

	if (click[0].type == SDL_MOUSEBUTTONUP) {
		keys[MOUSE_LEFT_BUTTION] = 0;
		keysChange[MOUSE_LEFT_BUTTION] = 0;
	}
	if (click[1].type == SDL_MOUSEBUTTONUP) {
		keys[MOUSE_MIDDLE_BUTTION] = 0;
		keysChange[MOUSE_MIDDLE_BUTTION] = 0;
	}
	if (click[2].type == SDL_MOUSEBUTTONUP) {
		keys[MOUSE_RIGHT_BUTTION] = 0;
		keysChange[MOUSE_RIGHT_BUTTION] = 0;
	}


}
void GameContener::ProcessEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{

		switch (event.type)
		{

		case SDL_KEYDOWN:
			HandleKeyDown(&event.key.keysym);
			break;

		case SDL_KEYUP:
			HandleKeyUp(&event.key.keysym);
			break;
		case SDL_MOUSEMOTION:
			MouseMotion(&event.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseClickDown(&event.button);
			break;
		case SDL_MOUSEBUTTONUP:
			MouseClickUp(&event.button);
			break;
		case SDL_QUIT:
			QuitGame();
			break;
		}
		if (event.type == SDL_WINDOWEVENT) {
			switch (event.window.event) {
			case SDL_WINDOWEVENT_SHOWN:
				//	SDL_Log("Window %d shown", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				//	SDL_Log("Window %d hidden", event.window.windowID);
				gamePause = true;
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				//	SDL_Log("Window %d exposed", event.window.windowID);
				gamePause = false;
				break;
			case SDL_WINDOWEVENT_MOVED:
				//SDL_Log("Window %d moved to %d,%d",event.window.windowID, event.window.data1,	event.window.data2);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				//	SDL_Log("Window %d resized to %dx%d",event.window.windowID, event.window.data1,	event.window.data2);
				resizeWindow(event.window.data1, event.window.data2);
				screen_width = event.window.data1;
				screen_height = event.window.data2;

				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				//	SDL_Log("Window %d size changed to %dx%d",event.window.windowID, event.window.data1,event.window.data2);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				//SDL_Log("Window %d minimized", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				//	SDL_Log("Window %d maximized", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				//SDL_Log("Window %d restored", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				//	SDL_Log("Mouse entered window %d",event.window.windowID);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				//	SDL_Log("Mouse left window %d", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				//	SDL_Log("Window %d gained keyboard focus",event.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				//	SDL_Log("Window %d lost keyboard focus",event.window.windowID);
				gamePause = true;
				break;

			}
		}
	}

	return;
}
void GameContener::StartEngine()
{
	SDL_Init(0);
	//	player->addWeapon("pistol", 0.5, true, 50, 300, 5, 12, 1000, 1000, "data/2/gun/Handgun_Game_Cycles_");

	animations = new GameAnimation();
	
	animations->addAllStartAnimation(screen_height,screen_width,text,mainWindow);

	map->initTerrain("data/heightMap.bmp","data/grass2.bmp","data/water.bmp", 0.1);
	player = new Player(vector3d(rand() % map->terrain->width, 20, -rand() % map->terrain->height), 0);

	player->addWeapon("pistol", 0.5, 1, 10, 300, 5, 12, 1000, 1000, animations, animationName::PistolNormal);
	player->addWeapon("AK", 0.3, 2, 15, 500, 5, 100, 80, 80, animations, animationName::AKNormal);
	player->addWeapon("Shotgun", 1.5, 1.5, 200, 20, 2, 2, 20, 20, animations, animationName::ShotgunNormal);


	enemy = new std::list<Enemy*>;
	for (int i = 0; i < lvl + 4;i++)
	enemy->push_back(new Enemy(100, 0.1, 10, collisionsphere(vector3d(rand() % map->terrain->width, 50, -rand() % map->terrain->height), 2), vector3d(0, 0, 0), player->getCamera()->getLocation(), animations, 1, 4, 0.5));

	bullets = new BulletFactory();

	mapElements = new WorldObjects();
	mapElements->addModel(animations->getAnimation(animationName::ObjectRock), 10, map->terrain->width, map->terrain->height,kind::flora);
	mapElements->addModel(animations->getAnimation(animationName::ObjectTree), 5, map->terrain->width, map->terrain->height, kind::flora);
	mapElements->addModel(animations->getAnimation(animationName::Portal), 1, map->terrain->width, map->terrain->height, kind::finish);

	for (int i = 0; i < mapElements->getSize(); i++) {
		vector3d tmp = mapElements->getPosition(i);
		tmp.changeY(map->getTerrainHeight(tmp.getX(), tmp.getZ()));
		mapElements->setHeight(i, tmp.getY());
	}

	return;
}

void GameContener::DoEngine()
{
	if (gamePause || !gameRunning)
		return;

	if (player->getHealth() >= 0) {
		player->update(keys, keysChange, map->getTerrainHeight(player->getX(), player->getZ()), mapElements, map, bullets);
		for (std::list<Enemy*>::iterator it = enemy->begin(); it != enemy->end(); it++) {
			(*it)->update(map->getTerrainHeight((*it)->getSphere()->center.getX(), (*it)->getSphere()->center.getZ()), mapElements, player->getCamera()->getLocation(), player->getRadius(), bullets);
		}
		//check that enemy is dead and add score to player
		std::list<Enemy*>::iterator it = enemy->begin();
		while (it != enemy->end()) {
			if ((*it)->isDead()) {
				player->addPoints((*it)->getStrength());
				
				
				//add ammo or  health bonus 
				int g = rand() % 8;
				if (g == 0)
					mapElements->addOneModel(animations->getAnimation(animationName::AddBullets), (*it)->getSphere()->center, kind::ammo);
				else if (g == 1)
					mapElements->addOneModel(animations->getAnimation(animationName::AddHealth), (*it)->getSphere()->center, kind::health);

				it = enemy->erase(it);
				enemy->push_back(new Enemy(100, 0.1, 10, collisionsphere(vector3d(rand() % map->terrain->width, 50, -rand() % map->terrain->height), 2), vector3d(0, 0, 0), player->getCamera()->getLocation(), animations, 1, 4, 0.5));

			}
			else
			{
				it++;
			}

		}
		mapElements->update();

		if (player->haveAnyGun()) {
			text->changeValues(player->getHealth(), player->getEnergy(), player->getCurrentWeapon()->getAmmoClip(), player->getCurrentWeapon()->getAllBullets(), player->getPoints(), player->getCurrentWeapon()->getName(), "0.lvl0", framespersecond);
		}
		else {
			text->changeValues(player->getHealth(), player->getEnergy(), 0, 0, player->getPoints(), "    ", "lvl: "+std::to_string(lvl), framespersecond);
		}
		if (player->isLevelEnd()) {


			map->~Map();
			map = new Map();
			map->initTerrain("data/heightMap.bmp", "data/grass2.bmp", "data/water.bmp", float(rand() % 25) / (float)100);

			mapElements->~WorldObjects();
			mapElements = new WorldObjects();
			mapElements->addModel(animations->getAnimation(animationName::ObjectRock), 8, map->terrain->width, map->terrain->height, kind::flora);
			mapElements->addModel(animations->getAnimation(animationName::ObjectTree), 2, map->terrain->width, map->terrain->height, kind::flora);
			mapElements->addModel(animations->getAnimation(animationName::Portal), 1, map->terrain->width, map->terrain->height, kind::finish);
			for (int i = 0; i < mapElements->getSize(); i++) {
				vector3d tmp = mapElements->getPosition(i);
				tmp.changeY(map->getTerrainHeight(tmp.getX(), tmp.getZ()));
				mapElements->setHeight(i, tmp.getY());
			}


			enemy->clear();
			for (int i = 0; i < lvl + 4; i++)
				enemy->push_back(new Enemy(100, 0.1, 10, collisionsphere(vector3d(rand() % map->terrain->width, 50, -rand() % map->terrain->height), 2), vector3d(0, 0, 0), player->getCamera()->getLocation(), animations, 1, 4, 0.5));

			lvl++;
			player->setNewLevelState();
			player->setPosition(vector3d( rand() % map->terrain->width -15, 20, -rand() % map->terrain->height+15));
			player->setStartPosition(player->getCamera()->getLocation());
			if (player->getHealth() < 100) {
				player->setHealth(100);
			}
			player->addPoints(1000);

			bullets->~BulletFactory();
			bullets = new BulletFactory();

		
		}
	}
	else {
		//text->renderText("GAME OVER", SDL_Color{ 255,0,0 }, coordinates::CENTER, 0, 0);

	}

	return;
}
void GameContener::WaitFrame(int fps)
{
	if (gamePause || !gameRunning)
		return;
	static int next_tick = 0;
	int this_tick;

	this_tick = SDL_GetTicks();
	if (this_tick < next_tick)
		SDL_Delay(next_tick - this_tick);
	next_tick = SDL_GetTicks() + (1000 / fps);

	return;
}
void GameContener::RenderPause()
{
	glClearColor(0.4, 0.7, 1.0, 0.5); //clear the screen to black
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
	glClearDepth(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//	glDisable(GL_LIGHTING);
	gluPerspective(45, screen_width / screen_height, 0.1, 500.0);

	text->renderText("Pause", { 255,0,100 }, coordinates::CENTER, 0, 0);
	SDL_GL_SwapWindow(mainWindow);

}


void GameContener::QuitGame()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	SDL_DestroyRenderer(renderer);
	// Destroy our window

	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
	//TODO: zrobiæ zrzut gry
	gameRunning = false;
	gamePause = true;
}
void GameContener::CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}
void GameContener::PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
