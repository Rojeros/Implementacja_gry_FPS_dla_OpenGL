#pragma once
#include <vector>
#include "ObjectContainer.h"
#include "Vector.h"
#include "CollisionPlane.h"
enum kind {
	flora,
	finish,
	health,
	ammo
};
struct ObjectInfo {
	vector3d position;
	vector3d scaling;
	ObjectContainer * data;
	CollisionPlane * coll[6];
	bool toDelete;
	kind type;
	ObjectInfo(vector3d position, vector3d scaling, ObjectContainer *data, kind type) :position(position), scaling(scaling), type(type),toDelete(false) {
		this->data = data;
		for (int i = 0; i < 6; i++) {
			if (data->collision->size() == 0) {	//if dont have collision on file
				coll[i] = new CollisionPlane(vector3d(),vector3d(),vector3d(),vector3d(),vector3d() );
			}else{
			coll[i] = new CollisionPlane((data->collision->at(i)->getnormal()),	//move collision to new position
				(data->collision->at(i)->get1point()*scaling.getX() + this->position),
				(data->collision->at(i)->get2point()*scaling.getX() + this->position),
				(data->collision->at(i)->get3point()*scaling.getX() + this->position),
				(data->collision->at(i)->get4point()*scaling.getX() + this->position)
				);
			}
		}

		}
	};
class WorldObjects
{
public:
	WorldObjects();
	void addModel(std::string path,int size,int mapX,int mapZ, kind type);
	void addModel(ObjectContainer * pointer, int size, int mapX, int mapZ, kind type);
	void addOneModel(ObjectContainer * pointer, vector3d position, kind type);
	vector3d getPosition(int number);
	void setHeight(int number, float height);
	void draw(float dt);
	void destroyObject(int number);
	void update();
	CollisionPlane * getCollisonPLane(int number,int iteration);
	kind getType(int number);
	int getSize();
	~WorldObjects();
private:
	//info about animation
	std::vector <ObjectContainer*> objects;
	//info about object on scene 
	std::vector <ObjectInfo> worldContainer;

};

