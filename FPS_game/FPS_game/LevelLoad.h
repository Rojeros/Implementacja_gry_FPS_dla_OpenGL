#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <vector>
#include "Vector.h"
#include "CollisionPlane.h"
#include "Object.h"



class LevelLoad
{
public:
	
	LevelLoad();
	Object* loadFromFile(std::string path);
	~LevelLoad();
private:
	std::vector<GLuint>normalIndices;
	std::vector<GLuint>	textureIndices;
	std::vector<std::string> loadedTextures;
	std::vector<int> loadedTexturesNum;
	
	bool ismaterial, isnormals, istexture, isvertexnormal;
	int parseMaterial(char* line, const std::string& filename, Object * newObject);
	int parseMaterialFile(char* line, std::string path, std::vector<std::string>* tmp);



};

