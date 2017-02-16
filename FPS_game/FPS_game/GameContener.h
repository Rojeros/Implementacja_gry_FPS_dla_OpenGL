#pragma once

#include <string>
#include <iostream>
#include "Camera.h"
#include <GL/glew.h>
#include <SDL.h>
#undef main
#include "Player.h"
#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include "LoadObjects.h"
#include "Object.h"
#include "ObjectContainer.h"
#include <cstdlib>
#include <ctime>
#include "WorldObjects.h"
#include "Collision.h"
#include "Enemy.h"
#include <list>
#include "BulletFactory.h"
#include "GameAnimation.h"
#include <iostream>
#include <vector>


class GameContener
{
public:
	GameContener();
	int Run();
	~GameContener();

private:
	friend class GameAnimation;
	static GLuint TextureID;
	// Our SDL_Window
	SDL_Window *mainWindow;
	// Our opengl context handle
	SDL_GLContext mainContext;
	SDL_Renderer *renderer = NULL;
	Player * player;
	Map * map;
	int lvl;
	bool keys[285];
	bool keysChange[285];
	int screen_width  ;
	int screen_height ;
	int full_screen;
	bool gameRunning;
	bool gamePause;
	float mouseSensitivity;
	float zoom;
	std::list<Enemy*> * enemy;
	BulletFactory * bullets;
	WorldObjects * mapElements;
	std::vector<material> materials;
	std::vector<materialVertex> materialsVertex;
	GameUI *text;
	GameAnimation * animations;
	bool SetOpenGLAttributes();
	void PrintSDL_GL_Attributes();
	void CheckSDLError(int line);
	void QuitGame();

	void HandleKeyDown(SDL_Keysym* keysym);
	void HandleKeyUp(SDL_Keysym* keysym);
	void MouseMotion(SDL_MouseMotionEvent * motion);
	void MouseClickDown(SDL_MouseButtonEvent * click);
	void MouseClickUp(SDL_MouseButtonEvent * click);
	void ProcessEvents(void);
	bool SetupRC();
	void StartEngine();
	void DoEngine();
	void Render();
	void WaitFrame(int fps);
	void RenderPause();


	static Uint32 lastTiks;
	// How many frames time values to keep
	// The higher the value the smoother the result is...
	// Don't make it 0 or less :)
#define FRAME_VALUES 100

	// An array to store frame times:
	Uint32 frametimes[FRAME_VALUES];

	// Last calculated SDL_GetTicks
	Uint32 frametimelast;

	// total frames rendered
	Uint32 framecount;

	// the value you want
	float framespersecond;

	// This function gets called once on startup.
	void fpsinit();
	void resizeWindow(int w, int h);
	void fpsthink();
};

