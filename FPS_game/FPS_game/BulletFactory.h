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
	Bullet(collisionsphere collision, vector3d directionVector, float speed, float maxLifeTime, float strength, ObjectContainer * loadedMesh) :collision(collision), directionVector(directionVector), speed(speed), maxLifeTime(maxLifeTime), strength(strength), lifeTime(0),toDelete(false) {
		mesh = loadedMesh;
	}
};

class BulletFactory
{
	ObjectContainer * animation;
	
	std::list<Bullet> bullets;
	float angle;
public:
	BulletFactory();
	void addBullet(collisionsphere collision, vector3d directionVector, float speed, float maxLifeTime, float strength, ObjectContainer * mesh);
	void update(float dt, Player * player, std::list<Enemy * > * enemy,Map * map);
	void draw();
	~BulletFactory();
};

#endif // !BULLET_FACTORY