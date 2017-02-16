#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <vector>
#include "Vector.h"
#include "Object.h"
#include "CollisionPlane.h"
#include "LoadObjects.h"

class ObjectContainer
{
public:
	ObjectContainer(std::string path,float time,bool isLooped);
	bool animationEnded();
	~ObjectContainer();
	void draw(float dt);
	std::vector<CollisionPlane*> * collision;
	void changeDurationOfAnimation(float time);
private:
	std::vector<Object*> *animation;
	int frames;
	bool displayed;
	int currentFrame;
	float time;
	float timePerFrame;
	std::vector<material> mainMaterial;
	std::vector<materialVertex> mainMaterialsVertex;
	std::string path;
	float duration;
	bool isLoop;
};

