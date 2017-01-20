#pragma once
#ifndef BULLET_FACTORY
#define BULLET_FACTORY


#include "Vector.h"
#include "Collision.h"
#include <vector>
#include <list>
#include "Player.h"
#include "Enemy.h"
#include "ObjectContainer.h"
#include "Map.h"
class Enemy;
struct Bullet
{
	float speed;
	vector3d directionVector;
	collisionsphere collision;
	float lifeTime;
	float maxLifeTime;
	float strength;
	bool toDelete;
	ObjectContainer * mesh;
	bool isFromPlayer;
	float angle;
	vector3d rotate;
	Bullet(collisionsphere collision, vector3d directionVector, float speed, float maxLifeTime, float strength, ObjectContainer * loadedMesh,bool isFromPlayer) :collision(collision), directionVector(directionVector), speed(speed), maxLifeTime(maxLifeTime), strength(strength), lifeTime(0),toDelete(false),isFromPlayer(isFromPlayer),angle(0) {
		mesh = loadedMesh;
		if(isFromPlayer){
		vector3d rotation(0, 0, 1);
	
		angle = rotation.dotproduct(directionVector);
		if (angle < -1)
			angle = -1;
		if (angle > 1)
			angle = 1;
		angle = (std::acos(angle / (rotation.length()*directionVector.length())));
		angle = angle*(float)180 / (float)M_PI;
		if (directionVector.x < 0) {
			angle = -angle;
		}
		rotate.change(0, 1, 0);
		}
		else {
			rotate.change(1,1,1);
		}
	}
};

class BulletFactory
{
	ObjectContainer * animation;
	
	std::list<Bullet> bullets;
public:
	BulletFactory();
	void addBullet(collisionsphere collision, vector3d directionVector, float speed, float maxLifeTime, float strength, ObjectContainer * mesh,bool isFromPlayer);
	void update(float dt, Player * player, std::list<Enemy * > * enemy,Map * map);
	void draw();
	~BulletFactory();
};

#endif // !BULLET_FACTORY