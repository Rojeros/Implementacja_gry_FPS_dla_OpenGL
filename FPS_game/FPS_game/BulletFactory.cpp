#include "BulletFactory.h"



BulletFactory::BulletFactory():angle(0)
{
}

void BulletFactory::addBullet(collisionsphere collision, vector3d directionVector, float speed, float maxLifeTime, float strength,ObjectContainer * mesh)
{
	bullets.push_back(Bullet( collision,  directionVector,  speed,  maxLifeTime, strength,mesh));
}

void BulletFactory::update(float dt, Player * player, std::list<Enemy*>* enemy,Map * map)
{
	for (std::list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++){
		it->lifeTime += dt;
		it->collision.center = it->collision.center + it->directionVector*it->speed;
		if (Collision::pointdistace(player->getCamera()->getLocation(), it->collision.center) < 1.5 + it->collision.r) {
			player->addHealth(-it->strength);
			it->toDelete = true;
		}
		if (map->getTerrainHeight(it->collision.center.x, it->collision.center.z) > it->collision.center.y) {
			it->toDelete = true;
		}

		for (std::list<Enemy*>::iterator itEnemy = enemy->begin(); itEnemy != enemy->end(); itEnemy++) {
			if (Collision::pointdistace((*itEnemy)->getSphere()->center, it->collision.center) < (*itEnemy)->getSphere()->r + it->collision.r) {
				(*itEnemy)->decreaseHealth(-it->strength);
				it->toDelete = true;
			}
		}
		//TODO: collision
	}
std::list<Bullet>::iterator it = bullets.begin();
while(it != bullets.end()){
	if (it->toDelete == true || it->lifeTime >= it->maxLifeTime) {
			it=bullets.erase(it);
	}
	else {
		it++;
	}
	}
}

void BulletFactory::draw()
{
	std::list<Bullet>::iterator it = bullets.begin();
	while (it != bullets.end()) {
		glPushMatrix();
		glTranslatef(it->collision.center.x, it->collision.center.y, it->collision.center.z);
		angle += 10;
		glRotated(angle, 1, 1, 1);
		it->mesh->draw(0.1);
		glPopMatrix();
			it++;
		
	}

}


BulletFactory::~BulletFactory()
{
}
