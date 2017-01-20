#include "GameAnimation.h"



GameAnimation::GameAnimation()
{
}

void GameAnimation::addAnimation(animationName name, std::string path,float time, bool looped)
{

	animations.insert(std::pair<animationName, ObjectContainer*>(name,new ObjectContainer(path,time,looped)) );
}

void GameAnimation::addAllStartAnimation()
{
	addAnimation(animationName::EnemyStand, "data/2/enemy/hero163.stand_", 1, false);
	addAnimation(animationName::EnemyAtack, "data/2/enemy/hero163.attack_", 1, true);
	addAnimation(animationName::EnemyMove, "data/2/enemy/hero163.move_", 1, true);
	addAnimation(animationName::EnemyBullet, "data/2/enemy/bullet.obj", 1, false);

	addAnimation(animationName::ObjectRock, "data/2/Rock1.obj", 1, false);
	addAnimation(animationName::ObjectTree, "data/2/Tree.obj", 1, false);

	addAnimation(animationName::PistolNormal, "data/2/gun/pistol/pistol.fire_000001.obj", 1, false);
	addAnimation(animationName::PistolFire, "data/2/gun/pistol/pistol.fire_", 1, true);
	addAnimation(animationName::PistolReload, "data/2/gun/pistol/pistol.reload_", 1, true);
	addAnimation(animationName::PistolBullet, "data/2/gun/pistol/bullet2.obj", 1, false);

	addAnimation(animationName::AKNormal, "data/2/gun/ak/ak.fire_000001.obj", 1, false);
	addAnimation(animationName::AKFire, "data/2/gun/ak/ak.fire_", 1, true);
	addAnimation(animationName::AKReload, "data/2/gun/ak/ak.reload_", 1, true);
	addAnimation(animationName::AKBullet, "data/2/gun/ak/bullet2.obj", 1, false);

	addAnimation(animationName::ShotgunNormal, "data/2/gun/shotgun/shotgun.fire_000001.obj", 1, false);
	addAnimation(animationName::ShotgunFire, "data/2/gun/shotgun/shotgun.fire_", 1, true);
	addAnimation(animationName::ShotgunReload, "data/2/gun/shotgun/shotgun.reload_", 1, true);
	addAnimation(animationName::ShotgunBullet, "data/2/gun/shotgun/bullet2.obj", 1, false);
	
}

void GameAnimation::changeAnimation(animationName toChange, std::string path, float time, bool looped)
{
	std::map<animationName, ObjectContainer*>::iterator iter;
	iter = animations.find(toChange);
	if (iter != animations.end()) {
		animations.erase(iter);
	}
	animations.insert(std::pair<animationName, ObjectContainer*>(toChange, new ObjectContainer(path, time, looped)));

}

ObjectContainer * GameAnimation::getAnimation(animationName name)
{
	std::map<animationName, ObjectContainer*>::iterator iter;
	iter = animations.find(name);
	return iter->second;
}


GameAnimation::~GameAnimation()
{
	std::map<animationName, ObjectContainer*>::iterator iter;
	iter = animations.begin();
	while (iter != animations.end()) {
		delete iter->second;
		iter++;
	}
}
