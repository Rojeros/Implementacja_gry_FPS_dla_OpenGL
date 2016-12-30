#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <vector>
#include "Vector.h"
#include "CollisionPlane.h"
/// <summary>	A material. </summary>
struct material {
	/// <summary>	The name. </summary>
	std::string name;

	/// <summary>	Gets the ni. </summary>
	///
	/// <value>	The ni. </value>

	float alpha, ns, ni;

	/// <summary>	Gets the spec[ 3]. </summary>
	///
	/// <value>	The spec[ 3]. </value>

	float dif[3], amb[3], spec[3];
	/// <summary>	The illum. </summary>
	int illum;
	/// <summary>	The texture. </summary>
	int texture;

	material(const char* na, float al, float n, float ni2, float* d, float* a, float* s, int i, int t);
};
struct materialVertex {
	int startIndex;
	int size;
	int materialIndex;
	int faceFormat;
	materialVertex(int si, int mi, int ff) :startIndex(si), size(0), materialIndex(mi), faceFormat(ff) {};

};

class Object
{
public:
	Object();
	~Object();
	void draw();

std::vector<float> vetrexes;
std::vector<float> normals;
std::vector<float> texturecoordinate;

std::vector<GLuint> vertexIndices;
		
std::vector<materialVertex> materialsVertex;
		
std::vector<material> materials;
std::vector<CollisionPlane> collplane;
	
private:

};

