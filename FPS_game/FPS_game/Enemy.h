#ifndef ENEMY_H
#define ENEMY_H
#include <Windows.h>
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include "vector.h"
#include "collisionplane.h"
#include "collision.h"
#include <string>
#include <vector>
#include <math.h>
#include "ObjectContainer.h"
#include "WorldObjects.h"

enum CurrentState
{
	wait = 0,
	walk = 1,
	attack = 2

};
/// <summary>	A collisionsphere. </summary>
class collisionsphere {
public:
	/// <summary>	The float to process. </summary>
	float r;
	/// <summary>	The center. </summary>
	vector3d center;

	/// <summary>	Constructor. </summary>
	///
	/// <param name="c">  	The vector3d to process. </param>
	/// <param name="rad">	The radians. </param>

	collisionsphere(vector3d c, float rad);
	/// <summary>	Default constructor. </summary>
	collisionsphere();
};


/// <summary>	A macro that defines pi. </summary>
//#define M_PI 1.57079632679489661923

/// <summary>	An enemy class, display, move, ang get info about enemys. </summary>
class Enemy 
{
	/// <summary>	The walk. </summary>
	ObjectContainer * walk;
	/// <summary>	The attack. </summary>
	ObjectContainer * attack;
	/// <summary>	The die. </summary>
	ObjectContainer * wait;
	ObjectContainer * currentAnimation;
	/// <summary>	The timer. </summary>
	unsigned int timer;
	/// <summary>	The health. </summary>
	int health;
	/// <summary>	The speed. </summary>
	float speed;
	/// <summary>	The strength. </summary>
	int strength;
	/// <summary>	The direction. </summary>
	vector3d direction;
	/// <summary>	The create struct. </summary>
	collisionsphere cs;
	/// <summary>	The rotation. </summary>
	vector3d rotation;
	vector3d rotation2;
	vector3d savedir;
	float angle;
	/// <summary>	true to isdead. </summary>
	bool isdead;
	/// <summary>	The dead timer. </summary>
	int deadTimer;
	vector3d force;
	CurrentState currentState;
	bool isCopy;

public:


	Enemy(int health, float speed, int strength, collisionsphere c, vector3d rot, vector3d playerloc,std::string path, float walkTime, float attackTime, float waitTime);

	Enemy(int health, float speed, int strength, collisionsphere c, vector3d rot, vector3d playerloc, Enemy * copy, float walkTime, float attackTime, float waitTime);

	~Enemy();
	bool update(float groundHeight, WorldObjects * collisions, vector3d playerpos);
	void show(float dt);
	collisionsphere* getSphere();

	void setLocation(vector3d& loc);
	bool setAttack(collisionsphere playerloc);
	void decreaseHealth(int num);
	int getHealth();
	int getStrength();
	bool isDead();
	bool deadTimerTick();
	int getDeadTimer();

};

#endif
