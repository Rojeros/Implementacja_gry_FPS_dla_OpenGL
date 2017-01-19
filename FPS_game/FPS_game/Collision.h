#pragma once
#include "vector.h"
#include <cmath>

/// <summary>	A collisionsphere. </summary>
class collisionsphere {
public:
	/// <summary>	The float to process. </summary>
	float r;
	/// <summary>	The center. </summary>
	vector3d center;

	/// <summary>	Constructor. </summary>
	///
	/// <param name="c">  	The vector3d to process. </param>
	/// <param name="rad">	The radians. </param>

	collisionsphere(vector3d c, float rad);
	/// <summary>	Default constructor. </summary>
	collisionsphere();
};


class Collision {
public:
	static bool sphereplane(vector3d& sp, vector3d pn, vector3d p1, vector3d p2, vector3d p3, vector3d p4, float r);
	static bool raysphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r, float* dist = NULL, vector3d* collpoint = NULL);
	static bool rayplane(const float& nx, float ny, float nz, float x0, float y0, float z0, float xs, float ys, float zs, float xd, float yd, float zd, vector3d p1, vector3d p2, vector3d p3, vector3d p4, float* dis = NULL, vector3d* point = NULL);
	static float rayplanedist(float nx, float ny, float nz, float x0, float y0, float z0, float xs, float ys, float zs, float xd, float yd, float zd);
	static float trianglearea(vector3d p1, vector3d p2, vector3d p3);
	static bool spheresphere(vector3d& c1, float r1, vector3d& c2, float r2);
	static float pointdistacesquare(vector3d p1, vector3d p2);
	static float pointdistace(vector3d p1, vector3d p2);
	static bool Collision::isCollision2points(float x, float z, vector2D vertices[3]);
};