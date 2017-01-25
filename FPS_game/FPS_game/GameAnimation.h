#pragma once
#ifndef GAME_ANIMATION
#define GAME_ANIMATION

#include "ObjectContainer.h"
#include <map>
#include "Text.h"
#include <SDL.h>


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
	ShotgunBullet,
	AddBullets,
	AddHealth,
	Portal
};
class GameAnimation
{
	std::map <animationName, ObjectContainer*> animations;
	void loadingScreen(float screen_height, float screen_width, GameUI * text, SDL_Window * mainWindow,int progress);
public:
	GameAnimation();
	void addAnimation(animationName name, std::string path, float time, bool looped);
	void addAllStartAnimation(float screen_height, float screen_width, GameUI * text, SDL_Window * mainWindow);
	void changeAnimation(animationName toChange, std::string path, float time, bool looped);
	ObjectContainer * getAnimation(animationName name);

	~GameAnimation();
};

#endif