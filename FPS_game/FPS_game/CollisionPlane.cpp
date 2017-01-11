#include "CollisionPlane.h"



CollisionPlane::CollisionPlane(float n1, float n2, float n3, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l)
{
	p[0].change(a, b, c);
	p[1].change(d, e, f);
	p[2].change(g, h, i);
	p[3].change(j, k, l);
	normal.change(n1, n2, n3);
}

CollisionPlane::CollisionPlane(vector3d normal, vector3d p1, vector3d p2, vector3d p3, vector3d p4)
{
	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	p[3] = p4;
	this->normal = normal;
}

void CollisionPlane::addAndScale(float a,float b)
{
	p[0] += vector3d(0, a, 0);
	p[1] += vector3d(0, a, 0);
	p[2] += vector3d(0, a, 0);
	p[3] += vector3d(0, a, 0);


}

vector3d CollisionPlane::get1point()
{
	return p[0];
}
vector3d CollisionPlane::get2point()
{
	return p[1];
}

vector3d CollisionPlane::get3point()
{
	return p[2];
}

vector3d CollisionPlane::get4point()
{
	return p[3];
}

vector3d CollisionPlane::getnormal()
{
	return normal;
}

bool CollisionPlane::isok()
{
	return (normal.x != 0 && normal.y != 0 && normal.z != 0);
}
