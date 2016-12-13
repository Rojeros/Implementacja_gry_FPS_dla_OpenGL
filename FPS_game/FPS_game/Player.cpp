#include "player.h"
#include <SDL.h>


Player::Player():health(500),energy(10), isSprint(false),points(0),dx(0),dy(0)
{
	
	force.change(0.0, -1, 0.0);
	//direction.change(0.0, 0.0, 0.0);
	cam = new Camera();

}



Camera* Player::getCamera()
{
	return cam;
}

void Player::show()
{
	cam->refresh();
}

void Player::jump()
{
	if (groundCollision)
	{
		groundCollision = false;
		direction.change(0.0, 2, 0.0);
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




void Player::update(bool * keys, float groundHeight)
{
	float modifier = 1;
	if (keys[SDL_SCANCODE_LSHIFT] == 1 || SDL_SCANCODE_RSHIFT == 1) {
		if (energy > 1) {
			isSprint = true;
			modifier = 3;
			
		}
	}

	if (keys[SDL_SCANCODE_LEFT] == 1|| keys[SDL_SCANCODE_A]) {
		cam->strafe(-1 * WALK_SPEED * modifier);
		if (isSprint)
			energy -= 0.05;
	}
	if (keys[SDL_SCANCODE_RIGHT] == 1 || keys[SDL_SCANCODE_D]) {
		cam->strafe(WALK_SPEED*modifier);
		if (isSprint)
			energy -= 0.05;
	}
	if (keys[SDL_SCANCODE_UP] == 1 || keys[SDL_SCANCODE_W]) {
		cam->move(WALK_SPEED*modifier);
		if (isSprint)
			energy -= 0.05;
	}
	if (keys[SDL_SCANCODE_DOWN] == 1 || keys[SDL_SCANCODE_S]) {
		cam->move(-WALK_SPEED*modifier);
		if (isSprint)
			energy -= 0.05;
	}
	if (keys[SDL_SCANCODE_F] == 1 ) {
		teleport();
		if (isSprint)
			energy -= 0.05;
	}
		cam->rotateYaw(0.005*dx);
		dx = 0;
		cam->rotatePitch(0.005*(-dy));
		dy = 0;
		
		if (getY() > groundHeight + 0.5){
			if ((getY() - groundHeight) < 1)
				direction += vector3d(0, (groundHeight- getY())+0.5, 0);
				
			if ((getY() - groundHeight) > 1)
			direction += force;
		}

	if (getY() < groundHeight+0.5 )
		direction += vector3d(0, groundHeight-getY()+0.5, 0);
	
	if (energy<10 && !isSprint)
		energy += 0.01;
	

		isSprint=false;

	vector3d newpos(cam->getLocation());

	newpos += direction;
	//if (newpos.getY() < groundHeight)
	//	newpos.changeY(groundHeight+0.5);

	cam->setLocation(newpos);
	direction.change(0, 0, 0);
	vector3d tmp(newpos);


}


void Player::decreaseHealth(int num)
{
	health -= num;
}

int Player::getHealth()
{
	return health;
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
	setHealth(1500);
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


