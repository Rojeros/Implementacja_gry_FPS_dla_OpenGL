#pragma once
#include "ObjectContainer.h"
#include <map>

enum animationName {
	EnemyStand,
	EnemyAtack,
	EnemyMove,
	EnemyBullet,
	ObjectRock,
	ObjectTree,
	PistolNormal,
	PistolFire,
	PistolReload,
	PistolBullet,
	AKNormal,
	AKFire,
	AKReload,
	AKBullet,
	ShotgunNormal,
	ShotgunFire,
	ShotgunReload,
	ShotgunBullet
};
class GameAnimation
{
	std::map <animationName, ObjectContainer*> animations;
public:
	GameAnimation();
	void addAnimation(animationName name, std::string path, float time, bool looped);
	void addAllStartAnimation();
	void changeAnimation(animationName toChange, std::string path, float time, bool looped);
	ObjectContainer * getAnimation(animationName name);
	~GameAnimation();
};

