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

static float LOOK_SPEED = 0.005;
static float WALK_SPEED = 0.1;

class Player

{
private:

	int health;
	vector3d force;
	vector3d direction;
	bool groundCollision;
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
	std::vector<Weapon> *arsenal;

public:
	Player();
	~Player();
	Camera* getCamera();
	void show(float dt);
	void jump();
	void lookAt(float dx, float dy);
	void Player::update(bool * keys, float groundHeight,WorldObjects * collisions);
	void decreaseHealth(int num);
	int getHealth();
	int getEnergy();
	void setHealth(int h);
	void addHealth(int h);
	void setStartPosition(vector3d pos);
	void addPoints(int num);
	Weapon *getCurrentWeapon();
	bool isGroundCollision();
	bool isDead();
	int getPoints();
	void teleport();
	void setPosition(vector3d position);
	void resetPlayer();
	float getX();
	float getZ();
	float getY();



};

#endif