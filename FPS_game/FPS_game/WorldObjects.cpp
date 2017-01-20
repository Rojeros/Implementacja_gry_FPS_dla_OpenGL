#include "WorldObjects.h"
#include <cstdlib>


WorldObjects::WorldObjects()
{
}

void WorldObjects::addModel(std::string path, int size, int mapX, int mapZ)
{
	objects.push_back(new ObjectContainer(path,5,false));
	for (int i = 0; i < size; i++) {
		float scale = (float)(rand() % 100) / (float)100 + 1.0f;
		worldContainer.push_back(ObjectInfo(vector3d(rand()%mapX,0,-rand()%mapZ),vector3d(scale, scale, scale),objects.back()));

	}
}

void WorldObjects::addModel(ObjectContainer * pointer, int size, int mapX, int mapZ)
{
	for (int i = 0; i < size; i++) {
		float scale = (float)(rand() % 100) / (float)100 + 1.0f;
		worldContainer.push_back(ObjectInfo(vector3d(rand() % mapX, 0, -rand() % mapZ), vector3d(scale, scale, scale), pointer));

	}
}

vector3d WorldObjects::getPosition(int number)
{
	return worldContainer[number].position;
}

void WorldObjects::setHeight(int number, float height)
{
	worldContainer[number].position.changeY(height-0.5);

	for (int i = 0; i < 6; i++) {
		worldContainer[number].coll[i]->addAndScale((height - 0.7), worldContainer[number].scaling.getX());
	}

}

void WorldObjects::draw(float dt)
{

	for (int i = 0; i < worldContainer.size(); i++) {
		glPushMatrix();
		glTranslatef(worldContainer[i].position.x, worldContainer[i].position.y , worldContainer[i].position.z);
		glScalef(worldContainer[i].scaling.x, worldContainer[i].scaling.y, worldContainer[i].scaling.z);
		worldContainer[i].data->draw(dt);
		
		glPopMatrix();
	}
}

CollisionPlane* WorldObjects::getCollisonPLane(int number,int iteration)
{
	return worldContainer.at(number).coll[iteration];
}


int WorldObjects::getSize()
{
	return worldContainer.size();
}


WorldObjects::~WorldObjects()
{
}
