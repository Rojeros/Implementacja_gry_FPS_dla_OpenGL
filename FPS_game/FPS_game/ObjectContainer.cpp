#include "ObjectContainer.h"



ObjectContainer::ObjectContainer(std::string path, float time, bool isLooped):currentFrame(0), duration(time),isLoop(isLooped), displayed(false),time(0)
{

	LoadObjects a;
	collision = new std::vector<CollisionPlane*>();
	animation=a.animation(path, mainMaterial, mainMaterialsVertex,collision,frames);
	
	timePerFrame = duration / (float)frames;
	if (timePerFrame <= 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "timePerFrame is NULL", NULL);
		exit(3);
	}
}

bool ObjectContainer::animationEnded()
{
	if (displayed == true) {
		displayed = false;
		return true;
	}
	return false;
}

ObjectContainer::~ObjectContainer()
{
	for (int i = 0; i < mainMaterial.size(); i++) {
		glDeleteTextures(1,&mainMaterial[i].texture);
	}


	for (int i = 0; i < animation->size(); i++) {
		if(animation->at(i)->vboId>0)
		glDeleteBuffersARB(1, &animation->at(i)->vboId);
		delete  animation->at(i);
	}
	delete animation;
	for (int i = 0; i < collision->size(); i++) {
		if (collision->at(i) != NULL)
		delete  collision->at(i);
	}
	delete collision;
}

void ObjectContainer::draw(float dt)
{

	if (frames > 0) {
		time += dt;

		if (time > timePerFrame) {
			do {
				time -= timePerFrame;
				currentFrame++;
			} while (time > timePerFrame);
		}



		if (currentFrame > frames - 1) {
			if (isLoop) {
				time = 0;
				currentFrame = 0;
				displayed = true;
			}
			else {
				currentFrame = frames - 1;
				displayed = true;
				
			}
		}
		animation->at(currentFrame)->draw(mainMaterial, mainMaterialsVertex);
	}
	else {
		animation->at(0)->draw(mainMaterial, mainMaterialsVertex);
		displayed = true;
	}
}

void ObjectContainer::changeDurationOfAnimation(float time)
{
	this->duration = time;
	timePerFrame = this->duration / (float)frames;
	if (timePerFrame <= 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "timePerFrame is NULL", NULL);
		exit(4);
	}
}
