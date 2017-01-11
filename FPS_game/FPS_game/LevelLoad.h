#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <vector>
#include "Vector.h"
#include "CollisionPlane.h"
#include "Object.h"
#include <sys/types.h>
#include <sys/stat.h>


class LevelLoad
{
public:

	LevelLoad();
	Object* loadFromFile(std::string path,bool isTexturFileIsLoad, std::vector<material> & mainMaterial, std::vector<materialVertex> &mainMaterialsVertex, std::vector<CollisionPlane*> *collision);
	std::vector<Object*> *animation(std::string path, std::vector<material> & mainMaterial, std::vector<materialVertex> &mainMaterialsVertex, std::vector<CollisionPlane*>* collision, int &frames);
	~LevelLoad();
private:
	std::vector<std::string> loadedTextures;
	std::vector<int> loadedTexturesNum;
	bool isCollisionLoad;
	std::string lastPath;
	bool ismaterial, isnormals, istexture, isvertexnormal;
	int parseMaterial(char* line, const std::string& filename, Object * newObject, std::vector<material> & mainMaterial);
	int parseMaterialFile(char* line, std::string path, std::vector<std::string>* tmp);



};

