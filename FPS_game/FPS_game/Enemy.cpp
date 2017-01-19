#include "Enemy.h"

collisionsphere::collisionsphere()
{
	center.x = center.y = center.z = 0;
	r = 0;
}

collisionsphere::collisionsphere(vector3d c, float r2)
{
	center = c;
	r = r2;
}

Enemy::Enemy(int health, float speed, int strength, collisionsphere c, vector3d rot, vector3d playerloc, std::string path, float walkTime, float attackTime, float waitTime)
{

	this->health = health;
	this->speed = speed;
	this->strength = strength;
	currentState = CurrentState::wait;
	isdead = false;
	cs = c;
	rotation = vector3d(0,0,1);
	angle=0;
	direction.change(playerloc - cs.center);
	direction.normalize();
	timer = 0;
	deadTimer = 0;
	force.change(0.0, -0.45, 0.0);
	walk = new ObjectContainer(path + ".move_", walkTime, true);
	attack = new ObjectContainer(path + ".attack_", attackTime, true);
	wait = new ObjectContainer(path + ".stand_", waitTime, true);
	bulletAnimation = new ObjectContainer(path + ".bullet_", attackTime, false);
	currentAnimation = wait;
	isCopy = false;
}

Enemy::Enemy(int health, float speed, int strength, collisionsphere c, vector3d rot, vector3d playerloc, Enemy * copy, float walkTime, float attackTime, float waitTime)
{
	this->health = health;
	this->speed = speed;
	this->strength = strength;
	currentState = CurrentState::wait;
	isdead = false;
	cs = c;
	rotation = vector3d(0, 0, 1);
	angle = 0;
	direction.change(playerloc - cs.center);
	direction.normalize();
	timer = 0;
	deadTimer = 0;
	force.change(0.0, -0.45, 0.0);
	walk = copy->walk;
	attack = copy->attack;
	wait = copy->wait;
	bulletAnimation = copy->bulletAnimation;
	currentAnimation = wait;
	isCopy = true;
}

Enemy::~Enemy()
{
	if(!isCopy){
	delete walk ;
	delete attack;
	delete wait;
	delete bulletAnimation;
	}
}

bool Enemy::update(float groundHeight, WorldObjects * collisions, vector3d playerpos, BulletFactory * bullets)
{
	if (health <= 0 || isdead) {
		isdead = true;
	}
	else {
		direction.change(0, 0, 0);
		if ((cs.center.getY() > groundHeight+cs.r))
			direction += force;

		if ((cs.center.getY() < groundHeight + cs.r*0.7))
			direction.changeY((-force.y*1.3));

		vector3d newpos(cs.center);
		newpos += direction;
		setLocation(newpos);


		//rotation2 = playerpos;
		direction.change(0, 0, 0);
		direction.change(playerpos - cs.center);
		//direction.changeY(0);
		direction.normalize();
		vector3d tmp(rotation);
		//tmp.changeY(0);
		tmp.normalize();

		angle = tmp.dotproduct(direction);
		if (angle < -1)
			angle = -1;
		if (angle > 1)
			angle = 1;
		angle = (std::acos(angle / (tmp.length()*direction.length())));
		angle = angle*(float)180 / (float)M_PI;
		if (playerpos.x < cs.center.x) {
			angle = -angle;
		}
		savedir = direction;

		//	rotation2 = rotation.crossproduct(direction);
		direction.change(0, 0, 0);

		switch (currentState) {
		case(CurrentState::wait) :
		{

			float distance = Collision::pointdistace(cs.center, playerpos);
			if (distance < 30 && (currentAnimation->animationEnded())) {
				currentState = CurrentState::attack;
				currentAnimation = attack;
			}
			else if (distance < 60 && (currentAnimation->animationEnded())) {
				currentState = CurrentState::walk;
				currentAnimation = walk;
			}
			else {

				//wait
			}
			direction.change(0, 0, 0);
			break;
		}
		case(CurrentState::walk) :
		{

		
			direction.change(playerpos - cs.center);
			//direction.changeY(0);
			direction.normalize();

			vector3d newpos(cs.center);
			newpos += direction*speed;
			if (collisions != NULL) {
				for (int i = 0; i < collisions->getSize(); i++) {
					for (int j = 0; j < 6; j++) {
						Collision::sphereplane(newpos,
							collisions->getCollisonPLane(i, j)->getnormal(),
							collisions->getCollisonPLane(i, j)->get1point(),
							collisions->getCollisonPLane(i, j)->get2point(),
							collisions->getCollisonPLane(i, j)->get3point(),
							collisions->getCollisonPLane(i, j)->get4point(), cs.r);

					}
				}
			}

			boolean isCollision = Collision::spheresphere(cs.center, cs.r, playerpos, 1.5);

			if (!isCollision) {
				setLocation(newpos);
			}
			direction.change(0, 0, 0);
			if (currentAnimation->animationEnded()) {
				currentState = CurrentState::wait;
				currentAnimation = wait;
			}

			break;
		}
		case(CurrentState::attack) :
		{
			
			if (currentAnimation->animationEnded()) {
				currentState = CurrentState::wait;
				currentAnimation = wait;
				direction.change(0, 0, 0);
				direction.change(playerpos - cs.center);
				direction.normalize();
				bullets->addBullet(collisionsphere(cs.center + direction*cs.r*1.5, 0.5), direction, 0.5, 10, strength,bulletAnimation);

			}
			break;
		}
		}
	}
	return 0;
}

void Enemy::show(float dt)
{
	//glPushMatrix();

	////glEnable(GL_NORMALIZE);
	//glBegin(GL_LINES);
	//glColor3f(1, 0.5, 0);
	//glVertex3f(cs.center.x, cs.center.y - cs.r + 0.5, cs.center.z);
	//glVertex3f(cs.center.x + 1000 * savedir.x, cs.center.y - cs.r + 0.5 + 1000 * savedir.y, cs.center.z + 1000 * savedir.z);
	//glEnd();
	//
	//glPopMatrix();

	glPushMatrix();
	


	glTranslatef(cs.center.x, cs.center.y - cs.r, cs.center.z);
//	static float v = 0;
	glRotatef(angle, 0, 1, 0);

	//v+=0.05;
	//if (v >= 360)
	//	v = 0;
	//std::cout << "" << v << " "<<angle<< "\n";
	/*std::system("cls");
	std::cout.width(6);
	std::cout<< v << " " << angle << " E " << cs.center.x << " " << cs.center.z << " P " << rotation2.x << " " << rotation2.z;*/
	//glBegin(GL_LINES);
	//glColor3f(0.5, 1, 0);
	//glVertex3f(0, 0,0);
	//glVertex3f(0+ 100 * rotation.x, 0 + 0.5 + 100 * rotation.y,0 + 100 * rotation.z);
	//glEnd();
//	glRotatef(rotation.x, 1, 0, 0);
//	glRotatef(rotation.z, 0, 0, 1);

	currentAnimation->draw(dt);


	glPopMatrix();
//	glDisable(GL_NORMALIZE);

}

collisionsphere* Enemy::getSphere()
{
	return &cs;
}

void Enemy::setLocation(vector3d& loc)
{
	cs.center = loc;
}

bool Enemy::setAttack(collisionsphere playerloc)
{

	if (Collision::spheresphere(cs.center, cs.r, playerloc.center, playerloc.r) && !isdead)
	{

		timer = 0;
		return 1;
	}
	else
	{

		//for (int i = 0; i < collisionPlanes.size(); i++)
		//{
		//	if (collision::sphereplane(playerloc.center, collisionPlanes[i].normal, collisionPlanes[i].p[0], collisionPlanes[i].p[1], collisionPlanes[i].p[2], collisionPlanes[i].p[3], playerloc.r))
		//{
		//	isattack = true;
		//	iswalk = true;
		//	timer = 0;
		//	return 1;
		//	}
		//}


		if (!isdead) {
			timer++;
			return 0;
		}

	}

	return 0;
}


void Enemy::decreaseHealth(int num)
{
	health += num;
}

int Enemy::getHealth()
{
	return health;
}

int Enemy::getStrength()
{
	return strength;
}

bool Enemy::isDead()
{
	if (health <= 0 || cs.center.y < -100) {
		isdead = true;
	}
	return isdead;
}

bool Enemy::deadTimerTick()
{
	deadTimer++;
	if (deadTimer > 10)
		return true;
	return false;
}

int Enemy::getDeadTimer()
{
	return deadTimer;
}

