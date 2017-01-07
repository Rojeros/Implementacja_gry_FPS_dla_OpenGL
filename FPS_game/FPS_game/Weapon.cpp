#include "weapon.h"
Weapon::Weapon(std::string name, unsigned int speed, bool isAutomatic, unsigned int power, unsigned int allBullets, unsigned int ammoClip, unsigned int maxMagazineBullets, float precision, float aimprecision, std::string path) {

	this->precision = precision;
	this->aimprecision = aimprecision;

	this->speed = speed;
	this->power = power;
	this->allBullets = allBullets;
	this->ammoClip = ammoClip;
	this->isAutomatic = isAutomatic;
	this->maxMagazineBullets = maxMagazineBullets;
	this->name = name;

	normalStateAnimation = new ObjectContainer("data/2/gun/Handgun_Game_Cycles_000004.obj", speed, true);
	fireStateAnimation = new ObjectContainer("data/2/gun/Handgun_Game_Cycles_000001.obj", speed, false);
	reloadStateAnimation = new ObjectContainer("data/2/gun/Handgun_Game_Cycles_000001.obj", speed, false);
	lastShot = 0;
	isAim = false;
	isRealoading = false;

	isFired = false;
	currentState = 1;
	setPosition(vector3d(-0.06, 0.13, 0.13));
	setRotation(vector3d(0, 0, 0));
	setCurrentPosition(vector3d(-0.06, 0.13, 0.13));
	setCurrentRotation(vector3d(0, 0, 0));

}

void Weapon::setName(std::string name_p) {
	name = name_p;
}

void Weapon::setCurrentPosition(vector3d currentPosition_p) {
	currentPosition = currentPosition_p;
}
void Weapon::setCurrentRotation(vector3d currentRotation_p) {
	currentRotation = currentRotation_p;
}
void Weapon::setPosition(vector3d position_p) {
	position = position_p;
}
void Weapon::setRotation(vector3d rotation_p) {
	rotation = rotation_p;
}
void Weapon::setMaxMagazineBullets(unsigned int maxMagazineBullets_p) {
	maxMagazineBullets = maxMagazineBullets_p;
}
void Weapon::setAmmoClip(unsigned int ammoClip_p) {
	ammoClip = ammoClip_p;
}
void Weapon::setAllBullets(unsigned int ammoClip_p) {
	allBullets = ammoClip_p;
}
void Weapon::setCurrentState(unsigned int currentState_p) {
	currentState = currentState_p;
}

void Weapon::setLastShot(unsigned int lastShot_p) {
	lastShot = lastShot_p;
}

void Weapon::addAllBullets(unsigned int ammoClip_p)
{
	allBullets += ammoClip_p;
}

std::string Weapon::getName() {
	return name;
}
unsigned int Weapon::getPower()
{
	return power;
}
vector3d Weapon::getCurrentPosition() {
	return currentPosition;
}
vector3d Weapon::getCurrentRotation() {
	return currentRotation;
}
vector3d Weapon::getPosition() {
	return position;
}
vector3d Weapon::getRotation() {
	return rotation;
}
unsigned int Weapon::getMaxMagazineBullets() {
	return maxMagazineBullets;
}
unsigned int Weapon::getAmmoClip() {
	return ammoClip;
}
unsigned int Weapon::getAllBullets()
{
	return allBullets;
}
unsigned int Weapon::getCurrentState() {
	return currentState;
}

unsigned int Weapon::getLastShot() {
	return lastShot;
}


void Weapon::update(vector3d newPosition) {

	lastShot++;
	currentPosition = newPosition;
	if (currentState != 1) {
		if (currentState == 2) {
			if (fireStateAnimation->animationEnded()) {
				currentState = 1;
				isFired = false;
			}
		}
		else {
			if (currentState == 3) {
				if (reloadStateAnimation->animationEnded()) {
					currentState = 1;
					isRealoading = false;
				}
			}
		}
	}


}
bool Weapon::fire(vector3d& direction, vector3d& camdirection) {
	if (isRealoading)
		return 0;
	if ((!isAutomatic && !isFired) || isAutomatic)
	{

		if (lastShot >= speed)
		{
			if (ammoClip > 0)
			{
				if (isAim)
				{
					direction.x = camdirection.x + ((float)(rand() % 2 - 1) / aimprecision);
					direction.y = camdirection.y + ((float)(rand() % 2 - 1) / aimprecision);
					direction.z = camdirection.z + ((float)(rand() % 2 - 1) / aimprecision);
				}
				else {
					direction.x = camdirection.x + ((float)(rand() % 2 - 1) / precision);
					direction.y = camdirection.y + ((float)(rand() % 2 - 1) / precision);
					direction.z = camdirection.z + ((float)(rand() % 2 - 1) / precision);
				}
				isFired = true;
				lastShot = 0;
				ammoClip--;

				currentState = 2;


				return 1;
			}

		}
	}
	return 0;
}
void Weapon::nofire()
{
	isFired = false;
}
void Weapon::reload() {
	if ((!isRealoading) && (allBullets > 0) && (ammoClip != maxMagazineBullets))
	{
		isRealoading = true;
		if (maxMagazineBullets < allBullets) {
			allBullets -= (maxMagazineBullets- ammoClip);
			ammoClip = maxMagazineBullets;
		}
		else {
			//niepe³ny magazynek
			ammoClip = allBullets;
			allBullets = 0;
		}


		currentState = 3;
	}
}
void Weapon::show(float angleYaw, float anglePitch, float dt) {
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();

	glTranslatef(currentPosition.x, currentPosition.y, currentPosition.z);
	//glRotatef(angle * 57.2957795, 1, 0, 0);
	glRotatef(-angleYaw * 57.2957795, 0, 1, 0);
	glRotatef(anglePitch * 57.2957795, 0, 0, 1);

	glTranslatef(1.4, -0.45, 0.5);
	glScalef(0.6, 0.6, 0.6);
	switch (currentState) {
	case(1) :
		normalStateAnimation->draw(dt);
		break;
	case(2) :
		fireStateAnimation->draw(dt);
		break;
	case(3) :
		reloadStateAnimation->draw(dt);
		break;
	}


	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}
