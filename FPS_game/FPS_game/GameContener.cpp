#include "GameContener.h"
#include "Player.h"
#include <iostream>

GameContener::GameContener() :full_screen(0), screen_width(512), screen_height(512),gameRunning(true)
{
	map = new Map();
}

int GameContener::Run()
{
	if (!SetupRC())
		return -1;

	StartEngine();
	while (gameRunning)
	{
		WaitFrame(60);
		ProcessEvents();
		DoEngine();
		Render();

	}

}


GameContener::~GameContener()
{
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
	if (full_screen == 1){
		flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
	}else{
		flags = SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE;
	}
	// Create our window 
	//TODO: zmieniæ na koniec 800 na sdl_windowpos_centered
	mainWindow = SDL_CreateWindow("FPS Game", 800, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, flags);

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


	return true;
}

void GameContener::Render()
{

	/* rysujemy tutaj */
	glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
	glClearDepth(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45, screen_width / screen_height, 0.1, 500.0);
	glBegin(GL_LINES);
	for (int i = 0; i <= 100; i++) {
		glColor3f(1, 0, 0);
		glVertex3f(100, 0, i);
		glVertex3f(-100, 0, i);
		glVertex3f(i, 0, 100);
		glVertex3f(i, 0, -100);
	};
	glEnd();
	player->update(keys,map->getTerrainHeight(player->getX(),player->getZ()));

	player->show();
	std::cout<<player->getCamera()->getLocation();
	map->renderTerrain();
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

	return;
}
void GameContener::MouseMotion(SDL_MouseMotionEvent * motion)
{
	//std::cout<<motion->xrel<<"\n";
	float dx= motion->x - screen_width / 2;
	float dy = motion->y - screen_width / 2;
	SDL_WarpMouseInWindow(mainWindow, (screen_width / 2), (screen_height / 2));
	player->lookAt(dx, dy);
	//int dy = motion->y - screen_height / 2;

}
void GameContener::MouseClick()
{
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
		case SDL_QUIT:
			QuitGame();
			break;
		}

	}

	return;
}
void GameContener::StartEngine()
{
	player = new Player();
	map->initTerrain("data/heightMap.bmp",0.1);
	/* inicjujemy engine tutaj */
	return;
}
void GameContener::DoEngine()
{

	/* engine tutaj */
	return;
}
void GameContener::WaitFrame(int fps)
{
	static int next_tick = 0;
	int this_tick;

	this_tick = SDL_GetTicks();
	if (this_tick < next_tick)
		SDL_Delay(next_tick - this_tick);
	next_tick = SDL_GetTicks() + (1000 / fps);

	return;
}
void GameContener::QuitGame()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit(); 
	//TODO: zrobiæ zrzut gry
	gameRunning = false;
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