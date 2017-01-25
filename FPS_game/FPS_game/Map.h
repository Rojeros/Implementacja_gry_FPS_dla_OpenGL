#pragma once
#include "Vector.h"
#include "Collision.h"
#include "CollisionPlane.h"
#include "FreeImage.h"

struct cellIndex
{
	int index_x, index_z, index_triangle;
};

typedef struct terrainT
{
	int terrainList;
	int terrainTexture[2];
	int width, height;
	vector3d **vertices;
	terrainT(int a, int b, int c) {
		terrainList = a;
		width = b;
		height = c;
		terrainTexture[0] = 0;
		terrainTexture[1] = 0;
	}
};


static vector3d getNormal(vector3d vertex1, vector3d vertex2, vector3d vertex3)
{
	vector3d v1, v2;
	vector3d result;

	v1.x = vertex1.x - vertex2.x;
	v1.y = vertex1.y - vertex2.y;
	v1.z = vertex1.z - vertex2.z;
	v2.x = vertex1.x - vertex3.x;
	v2.y = vertex1.y - vertex3.y;
	v2.z = vertex1.z - vertex3.z;

	result.x = v1.y*v2.z - v1.z*v2.y;
	result.y = v1.z*v2.x - v1.x*v2.z;
	result.z = v1.x*v2.y - v1.y*v2.x;

	result.normalize();
	return result;
}
class Map
{

public:
	Map();
	~Map();
	terrainT *terrain;
	CollisionPlane * box[6];


	void loadHeightMap(char *fileName, float heightFactor);
	void initTerrain(char *fileName,char*floorTexture,char*horizonTexture, float heightFactor);
	void freeTerrain();
	void renderTerrain();
	CollisionPlane ** getBox();
	float getTerrainHeight( float x, float z);
	cellIndex Map::getActiveTriangle( float x, float z);
	static vector2D lastPosition;
	static float lastHeiht;
};

