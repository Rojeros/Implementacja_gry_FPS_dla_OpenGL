#include "GameAnimation.h"



GameAnimation::GameAnimation()
{
}

void GameAnimation::addAnimation(animationName name, std::string path,float time, bool looped)
{

	animations.insert(std::pair<animationName, ObjectContainer*>(name,new ObjectContainer(path,time,looped)) );
}

void GameAnimation::addAllStartAnimation(float screen_height, float screen_width, GameUI * text, SDL_Window * mainWindow)
{
	
	int progress = 0;
	loadingScreen(screen_height, screen_width, text, mainWindow, progress);
	addAnimation(animationName::EnemyStand, "data/2/enemy/hero163.stand_", 1, false);
	progress += 5;
	loadingScreen(screen_height, screen_width, text, mainWindow, progress);
	progress += 5;
	addAnimation(animationName::EnemyAtack, "data/2/enemy/hero163.attack_", 1, true);
	loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	progress += 5;
	addAnimation(animationName::EnemyMove, "data/2/enemy/hero163.move_", 1, true);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::EnemyBullet, "data/2/enemy/bullet.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::ObjectRock, "data/2/Rock1.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::ObjectTree, "data/2/Tree.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::PistolNormal, "data/2/gun/pistol/pistol.fire_000001.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::PistolFire, "data/2/gun/pistol/pistol.fire_", 1, true);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::PistolReload, "data/2/gun/pistol/pistol.reload_", 1, true);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::PistolBullet, "data/2/gun/pistol/bullet2.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::AKNormal, "data/2/gun/ak/ak.fire_000001.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::AKFire, "data/2/gun/ak/ak.fire_", 1, true);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::AKReload, "data/2/gun/ak/ak.reload_", 1, true);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::AKBullet, "data/2/gun/ak/bullet2.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;

	addAnimation(animationName::ShotgunNormal, "data/2/gun/shotgun/shotgun.fire_000001.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::ShotgunFire, "data/2/gun/shotgun/shotgun.fire_", 1, true);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::ShotgunReload, "data/2/gun/shotgun/shotgun.reload_", 1, true);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	addAnimation(animationName::ShotgunBullet, "data/2/gun/shotgun/bullet2.obj", 1, false);
	 loadingScreen(screen_height,  screen_width, text, mainWindow,progress);
	 progress += 5;
	 addAnimation(animationName::AddBullets, "data/2/ammo.obj", 1, false);
	 loadingScreen(screen_height, screen_width, text, mainWindow, progress);
	 progress += 5;
	addAnimation(animationName::AddHealth, "data/2/aid.obj", 1, false);
	loadingScreen(screen_height, screen_width, text, mainWindow, progress);
	progress += 5;
	addAnimation(animationName::Portal, "data/2/portal.obj", 1, false);
	loadingScreen(screen_height, screen_width, text, mainWindow, progress);
	
		 
}

void GameAnimation::changeAnimation(animationName toChange, std::string path, float time, bool looped)
{
	std::map<animationName, ObjectContainer*>::iterator iter;
	iter = animations.find(toChange);
	if (iter != animations.end()) {
		animations.erase(iter);
	}
	animations.insert(std::pair<animationName, ObjectContainer*>(toChange, new ObjectContainer(path, time, looped)));

}

ObjectContainer * GameAnimation::getAnimation(animationName name)
{
	std::map<animationName, ObjectContainer*>::iterator iter;
	iter = animations.find(name);
	return iter->second;
}
void GameAnimation::loadingScreen(float screen_height, float screen_width, GameUI * text, SDL_Window * mainWindow, int progress)
{

	float margin = screen_height *0.2;
	float xl = margin;
	float xr = margin;
	float	y = margin;

	float  minusLife = (float)(100 - progress) / 100;
	float boxLifeWidth = screen_width*0.6;
	float height = screen_height *0.1;
	boxLifeWidth = boxLifeWidth - boxLifeWidth*minusLife;
	float  green = 1 - minusLife;
	float  redLife = minusLife;



	/* rysujemy tutaj */
	glClearColor(0, 0, 0, 0); //clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer

	text->renderText("Loading objects", { 0,200,50 }, coordinates::CENTER, 0, 0);
	text->renderText("Please wait", { 0,200,50 }, coordinates::CENTER, -1, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, screen_width, 0, screen_height); // m_Width and m_Height is the resolution of window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl, y);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl + boxLifeWidth, y);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl + boxLifeWidth, y + height);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl, y + height);
	glEnd();



	glEnable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	//SDL_Delay(100);



	SDL_GL_SwapWindow(mainWindow);

}

GameAnimation::~GameAnimation()
{

	for (std::map<animationName, ObjectContainer*>::iterator itr = animations.begin(); itr != animations.end(); itr++){
		delete itr->second;
		
	}
}
