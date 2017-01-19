#include "player.h"
#include <SDL.h>


Player::Player(vector3d position,int points):health(100),energy(100), isSprint(false),points(points),dx(0),dy(0), isJump(false), jumpHeight(0)
{
	
	force.change(0.0, -0.45, 0.0);
	//direction.change(0.0, 0.0, 0.0);
	cam = new Camera();
	cam->setLocation(position);
	startPoint = position;
	arsenal = new std::vector<Weapon*>();
	currentWeaponNUmber = 0;


}

Player::~Player()
{

	//TODO: delete all arsenal
	//delete cam;
}



Camera* Player::getCamera()
{
	return cam;
}

void Player::show(float dt)
{
	cam->refresh();
	
}

void Player::jump()
{
	if (groundCollision || isJump)
	{
		groundCollision = false;
		isJump = true;

		jumpHeight += 0.2;
		direction.change(0, jumpHeight, 0);
		if (jumpHeight >= 2.5) {
			isJump = false;
			jumpHeight = 0;
		}

	}
}

void Player::lookAt(float dx, float dy)
{
	this->dx += dx;
	this->dy += dy;
}

void Player::teleport()
{
	direction.change(0.0, 10, 0.0);
}




void Player::update(bool * keys, bool * keysChange, float groundHeight, WorldObjects * collisions, Map*map)
{
	float modifier = 1;
	if (keys[SDL_SCANCODE_LSHIFT] == 1 || SDL_SCANCODE_RSHIFT == 1) {
		if (energy > 20) {
			isSprint = true;
			modifier = 3;
			
		}
	}

	if (keys[SDL_SCANCODE_LCTRL] == 1) {
		addHealth(-1);

		
	}

	if (keys[SDL_SCANCODE_LEFT] == 1|| keys[SDL_SCANCODE_A]) {
		cam->strafe(-1 * WALK_SPEED * modifier);
		if (isSprint)
			energy -= 0.5;
	}
	if (keys[SDL_SCANCODE_RIGHT] == 1 || keys[SDL_SCANCODE_D]) {
		cam->strafe(WALK_SPEED*modifier);
		if (isSprint)
			energy -= 0.5;
	}
	if (keys[SDL_SCANCODE_UP] == 1 || keys[SDL_SCANCODE_W]) {
		cam->move(WALK_SPEED*modifier);
		if (isSprint)
			energy -= 0.5;
	}
	if (keys[SDL_SCANCODE_DOWN] == 1 || keys[SDL_SCANCODE_S]) {
		cam->move(-WALK_SPEED*modifier);
		if (isSprint)
			energy -= 0.5;
	}
	if (keys[SDL_SCANCODE_F] == 1 ) {
	//	teleport();
		}
	if (keys[SDL_SCANCODE_R] == 1 && keysChange[SDL_SCANCODE_R]==0) {
		if (!arsenal->empty()){
		arsenal->at(currentWeaponNUmber)->reload();
		keysChange[SDL_SCANCODE_R] = 1;
		}
	}
	if (keys[SDL_SCANCODE_SPACE] == 1) {
		jump();
	}
	if (keys[SDL_SCANCODE_Q] == 1 && keysChange[SDL_SCANCODE_Q] == 0) {
		if (!arsenal->empty()){
			previousWeapon();
			keysChange[SDL_SCANCODE_Q] = 1;
		}
	}
	if (keys[SDL_SCANCODE_E] == 1 && keysChange[SDL_SCANCODE_E] == 0) {
		if (!arsenal->empty()){
			nextWeapon();
			keysChange[SDL_SCANCODE_E] = 1;
		}
	}



		cam->rotateYaw(0.005*dx);
		dx = 0;
		cam->rotatePitch(0.005*(-dy));
		dy = 0;



		if (getY() > groundHeight + 0.5){
			if ((getY() - groundHeight) < 1){
				direction += vector3d(0, (groundHeight- getY())+0.5, 0);
				groundCollision = true;
			}
			if ((getY() - groundHeight) > 1)
			direction += force;
			
		}

	if (getY() < groundHeight+0.5 ){
		direction += vector3d(0, groundHeight-getY()+0.5, 0);
		groundCollision = true;
	}
	if (energy<100 && !isSprint)
		energy += 0.1;
	

		isSprint=false;

	vector3d newpos(cam->getLocation());

	newpos += direction;
	
	if (collisions != NULL) {
		for (int i = 0; i < collisions->getSize(); i++) {
			for (int j = 0; j < 6; j++) {
			Collision::sphereplane(newpos,
					collisions->getCollisonPLane(i, j)->getnormal(),
					collisions->getCollisonPLane(i, j)->get1point(),
					collisions->getCollisonPLane(i, j)->get2point(),
					collisions->getCollisonPLane(i, j)->get3point(),
					collisions->getCollisonPLane(i, j)->get4point(), 1.5);
		

				
			}
			//	std::cout << collisions->getCollisonPLane(i, j)->get1point() << " " << collisions->getCollisonPLane(i, j)->get2point() << " " << collisions->getCollisonPLane(i, j)->get3point() << " " << collisions->getCollisonPLane(i, j)->get4point() << "\n";
		}
	}
	for (int i = 0; i < 6; i++) {
		Collision::sphereplane(newpos,
			map->getBox()[i]->getnormal(),
			map->getBox()[i]->get1point(),
			map->getBox()[i]->get2point(),
			map->getBox()[i]->get3point(),
			map->getBox()[i]->get4point(), 1.5);
	}
	cam->setLocation(newpos);
	direction.change(0, 0, 0);

	if (!arsenal->empty())
	arsenal->at(currentWeaponNUmber)->update(newpos);

}


void Player::decreaseHealth(int num)
{
	health -= num;
}

int Player::getHealth()
{
	return health;
}

int Player::getEnergy()
{
	return energy;
}


void Player::setHealth(int h)
{
	health = h;
}


void Player::addHealth(int h)
{
	health += h;
}



void Player::setStartPosition(vector3d pos)
{
	startPoint = pos;
}



void Player::addPoints(int num)
{
	points += num;
}



Weapon * Player::getCurrentWeapon()
{
	if (arsenal != NULL)
	return arsenal->at(currentWeaponNUmber);
}

void Player::addWeapon(std::string name, unsigned int speed, bool isAutomatic, unsigned int power, unsigned int allBullets, unsigned int ammoClip, unsigned int maxMagazineBullets, float precision, float aimprecision, std::string path)
{
	arsenal->push_back(new Weapon(name,speed,isAutomatic,power,allBullets,ammoClip,maxMagazineBullets,precision,aimprecision,path));
	currentWeaponNUmber = arsenal->size() - 1;
}

void Player::nextWeapon()
{
	if (!arsenal->empty()) {
		currentWeaponNUmber++;
		if (currentWeaponNUmber>arsenal->size()-1) {
			currentWeaponNUmber = 0;
		}
	}
}

void Player::previousWeapon()
{
	if (arsenal != NULL) {
		currentWeaponNUmber--;
		if (currentWeaponNUmber<0 ) {
			currentWeaponNUmber = arsenal->size() - 1;
		}
	}
}

bool Player::haveAnyGun()
{
	if (!arsenal->empty()) {
	return true;
	}
	else {
		return false;
	}
}

int Player::getPoints()
{
	return points;
}

void Player::setPosition(vector3d position)
{
	cam->setLocation(position);
}


bool Player::isDead()
{
	if (health <= 0 || cam->getLocation().y<-100)
		return true;

	return false;
}

void Player::resetPlayer()
{
	setHealth(100);
	setPosition(startPoint);
	points = 0;
}

float Player::getX()
{
	return cam->getLocation().getX();
}

float Player::getZ()
{
	return cam->getLocation().getZ();
}

float Player::getY()
{
	return cam->getLocation().getY();
}


