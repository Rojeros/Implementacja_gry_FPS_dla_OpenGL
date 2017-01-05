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
#include "Text.h"
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
	SDL_Renderer *renderer = NULL;

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
	GameUI *text;
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
	void fpsinit() {

		// Set all frame times to 0ms.
		memset(frametimes, 0, sizeof(frametimes));
		framecount = 0;
		framespersecond = 0;
		frametimelast = SDL_GetTicks();

	}

	void fpsthink() {

		Uint32 frametimesindex;
		Uint32 getticks;
		Uint32 count;
		Uint32 i;

		// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
		// This value rotates back to 0 after it hits FRAME_VALUES.
		frametimesindex = framecount % FRAME_VALUES;

		// store the current time
		getticks = SDL_GetTicks();

		// save the frame time value
		frametimes[frametimesindex] = getticks - frametimelast;

		// save the last frame time for the next fpsthink
		frametimelast = getticks;

		// increment the frame count
		framecount++;

		// Work out the current framerate

		// The code below could be moved into another function if you don't need the value every frame.

		// I've included a test to see if the whole array has been written to or not. This will stop
		// strange values on the first few (FRAME_VALUES) frames.
		if (framecount < FRAME_VALUES) {

			count = framecount;

		}
		else {

			count = FRAME_VALUES;

		}

		// add up all the values and divide to get the average frame time.
		framespersecond = 0;
		for (i = 0; i < count; i++) {

			framespersecond += frametimes[i];

		}

		framespersecond /= count;

		// now to make it an actual frames per second value...
		framespersecond = 1000.f / framespersecond;

	}
};

