#pragma once

#include <vector>
#include "vector.h"
#include <GL/glew.h>
#include <SDL.h>
#include "ObjectContainer.h"
#include "GameAnimation.h"

class Weapon {

	std::string name;
	ObjectContainer * normalStateAnimation;
	ObjectContainer * fireStateAnimation;
	ObjectContainer * reloadStateAnimation;
	ObjectContainer * bulletAnimation;
	unsigned int allBullets;
	bool isFired;
	bool isRealoading;
	bool isAim;
	bool isCopy;

	vector3d currentPosition;
	vector3d currentRotation;
	vector3d position;
	vector3d rotation;
	int maxMagazineBullets;
	int ammoClip;
	unsigned int speed;
	float precision;
	float aimprecision;
	unsigned int power;
	unsigned int currentState;
	float lastShot;
	int startAmmo;

public:
	Weapon();
	Weapon(std::string name, unsigned int power, unsigned int allBullets, unsigned int ammoClip, unsigned int maxMagazineBullets, float precision, float aimprecision,std::string path);
	Weapon(std::string name,  float speedOfFireAnimation, float speedOfReloadAnimation, unsigned int power, unsigned int allBullets, unsigned int ammoClip, unsigned int maxMagazineBullets, float precision, float aimprecision, GameAnimation * copy, animationName weaponType);

	~Weapon();
	void setName(std::string name_p);
	
	void setCurrentPosition(vector3d currentPosition_p);
	void setCurrentRotation(vector3d currentRotation_p);
	void setPosition(vector3d position_p);
	void setRotation(vector3d rotation_p);
	void setMaxMagazineBullets(unsigned int maxMagazineBullets_p);
	void setAllBullets(unsigned int ammoClip_p);
	void setAmmoClip(unsigned int ammoClip_p);
	void setCurrentState(unsigned int currentState_p);
	void addAllBullets(unsigned int ammoClip_p);
	std::string getName();
	unsigned int getPower();
	vector3d getCurrentPosition();
	vector3d getCurrentRotation();
	vector3d getPosition();
	vector3d getRotation();
	ObjectContainer * getBulletAnimation();
	unsigned int getMaxMagazineBullets();
	unsigned int getAmmoClip();
	unsigned int getAllBullets();
	unsigned int getCurrentState();
	void update(vector3d newPosition);
	bool fire(vector3d& direction, vector3d& camdirection);
	void nofire();
	void reload();
	void resetAmmo();
	void show(float angleYaw, float anglePitch, float dt);

};