#pragma once
#ifdef WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>
#include "Camera.h"
#include <GL/glew.h>
#include <SDL.h>
#include "Player.h"
#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include "LevelLoad.h"
#include "Object.h"

class GameContener
{
public:
	GameContener();
	int Run();
	~GameContener();

private:
	static GLuint TextureID;
	// Our SDL_Window
	SDL_Window *mainWindow;
	// Our opengl context handle
	SDL_GLContext mainContext;
	Player * player;
	Map * map;
	bool keys[282];
	int screen_width  ;
	int screen_height ;
	int full_screen;
	bool gameRunning;
	std::vector<Object*> enemy;
	std::vector<material> materials;
	std::vector<materialVertex> materialsVertex;

	bool SetOpenGLAttributes();
	void PrintSDL_GL_Attributes();
	void CheckSDLError(int line);
	void QuitGame();

	void HandleKeyDown(SDL_Keysym* keysym);
	void HandleKeyUp(SDL_Keysym* keysym);
	void MouseMotion(SDL_MouseMotionEvent * motion);
	void MouseClick();
	void ProcessEvents(void);
	bool SetupRC();
	void StartEngine();
	void DoEngine();
	void Render();
	void WaitFrame(int fps);

};

