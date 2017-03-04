#pragma once
#ifndef gracz_h
#define gracz_h
#include "vector.h"
#include <vector>
#include <string>
#include <cmath>
#include "camera.h"
#include "Weapon.h"
#include "WorldObjects.h"
#include "Collision.h"
#include "Map.h"
#include "BulletFactory.h"

class BulletFactory;

static float LOOK_SPEED = 0.005;
static float WALK_SPEED = 0.3;
static float ADD_HEIGHT = 2.8;

#define MOUSE_LEFT_BUTTION 282
#define MOUSE_MIDDLE_BUTTION 283
#define MOUSE_RIGHT_BUTTION 284


class Player

{
private:

	int health;
	vector3d force;
	vector3d direction;
	bool groundCollision;
	bool beHit;
	float hitTimeDisplay;
	bool isSprint;
	float energy;
	int points;
	bool wallCollision;
	bool isJump;
	float jumpHeight;
	vector3d startPoint;
	Camera * cam;
	float dx, dy;
	float dvx, dvy;
	std::vector<Weapon*> *arsenal;
	int currentWeaponNUmber;
	float r;
	bool isLevelFinished;

public:
	Player(vector3d position, int points);
	~Player();
	Camera* getCamera();
	void show(float dt);
	void jump();
	void lookAt(float dx, float dy);
	void Player::update(bool * keys,bool * keyChange, float groundHeight,WorldObjects * collisions,Map*map,BulletFactory * bullets);
	void decreaseHealth(int num);
	int getHealth();
	int getEnergy();
	void setHealth(int h);
	void addHealth(int h);
	void setStartPosition(vector3d pos);
	void addPoints(int num);
	Weapon *getCurrentWeapon();
	void addWeapon(std::string name, unsigned int power, unsigned int allBullets, unsigned int ammoClip, unsigned int maxMagazineBullets, float precision, float aimprecision, std::string path);
	void addWeapon(std::string name, float fireAnimationSpeed, float reloadAnimationSpeed, unsigned int power, unsigned int allBullets, unsigned int ammoClip, unsigned int maxMagazineBullets, float precision, float aimprecision, GameAnimation * copy, animationName weaponType);
	void nextWeapon();
	void previousWeapon();
	bool haveAnyGun();
	void resetArsenal();
	bool isDead();
	int getPoints();
	void teleport();
	void setPosition(vector3d position);
	void resetPlayer();
	float getX();
	float getZ();
	float getY();
	float getRadius();
	bool isLevelEnd();
	bool wasHit(float dt);
	void setHit(bool state);
	void setNewLevelState();


};

#endif