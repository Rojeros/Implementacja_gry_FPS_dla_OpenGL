#pragma once
#include "Vector.h"

class CollisionPlane
{
public:
public:
	/// <summary>	The p[ 4]. </summary>
	/// <summary>	The p[ 4]. </summary>
	vector3d p[4];
	/// <summary>	The normal. </summary>
	vector3d normal;

	/// <summary>	Constructor. </summary>
	///
	/// <param name="n1">	The first float. </param>
	/// <param name="n2">	The second float. </param>
	/// <param name="n3">	The third float. </param>
	/// <param name="a"> 	The float to process. </param>
	/// <param name="b"> 	The float to process. </param>
	/// <param name="c"> 	The float to process. </param>
	/// <param name="d"> 	The float to process. </param>
	/// <param name="e"> 	The float to process. </param>
	/// <param name="f"> 	The float to process. </param>
	/// <param name="g"> 	The float to process. </param>
	/// <param name="h"> 	The height. </param>
	/// <param name="i"> 	Zero-based index of the. </param>
	/// <param name="j"> 	The float to process. </param>
	/// <param name="k"> 	The float to process. </param>
	/// <param name="l"> 	The float to process. </param>

	CollisionPlane(float n1, float n2, float n3, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l);
	CollisionPlane(vector3d normal,vector3d p1,vector3d p2,vector3d p3,vector3d p4);

	/// <summary>	Gets the 1point. </summary>
	///
	/// <returns>	The 1point. </returns>
	void addAndScale(float a, float b);
	vector3d get1point();

	/// <summary>	Gets the 2point. </summary>
	///
	/// <returns>	The 2point. </returns>

	vector3d get2point();

	/// <summary>	Gets the 3point. </summary>
	///
	/// <returns>	The 3point. </returns>

	vector3d get3point();

	/// <summary>	Gets the 4point. </summary>
	///
	/// <returns>	The 4point. </returns>

	vector3d get4point();

	/// <summary>	Gets the getnormal. </summary>
	///
	/// <returns>	A vector3d. </returns>

	vector3d getnormal();

	/// <summary>	Query if this object isok. </summary>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>

	bool isok();
};

