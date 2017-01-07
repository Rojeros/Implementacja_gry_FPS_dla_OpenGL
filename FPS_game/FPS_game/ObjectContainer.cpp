#include "ObjectContainer.h"



ObjectContainer::ObjectContainer(std::string path, float time, bool isLooped):currentFrame(0), duration(time),isLoop(isLooped), displayed(false)
{

	LevelLoad a;
	animation=a.animation(path, mainMaterial, mainMaterialsVertex,frames);

	timePerFrame = time / (float)frames;

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
	for (int i = 0; i < frames; i++) {
		glDeleteBuffersARB(1, &animation->at(i)->vboId);
		delete  animation->at(i);
	}
	delete animation;
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
