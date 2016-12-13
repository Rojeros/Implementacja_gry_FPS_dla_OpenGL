#include "Collision.h"


bool Collision::raysphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r, float* dist, vector3d* collpoint)
{


	float b = 2 * (xd*(xs - xc) + yd*(ys - yc) + zd*(zs - zc));
	float c = xs*xs - 2 * xs*xc + xc*xc + ys*ys - 2 * ys*yc + yc*yc + zs*zs - 2 * zs*zc + zc*zc - r*r;
	float disc = (b*b - 4 * c);
	if (disc<0)	//if the discriminant is less then 0, there is no intersection
		return false;
	else {
		if (dist != NULL)
		{
			(*dist) = (-b + sqrt(disc)) / 2;
			if (collpoint != NULL)
			{
				float x = xs + (*dist)*xd;
				float y = ys + (*dist)*yd;
				float z = zs + (*dist)*zd;
				collpoint->change(x, y, z);
			}
		}
		return true;
	}
}

float Collision::trianglearea(vector3d p1, vector3d p2, vector3d p3)
{
	//area of the triangle with the heron fomula
	float a = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y) + (p2.z - p1.z)*(p2.z - p1.z));
	float b = sqrt((p3.x - p2.x)*(p3.x - p2.x) + (p3.y - p2.y)*(p3.y - p2.y) + (p3.z - p2.z)*(p3.z - p2.z));
	float c = sqrt((p1.x - p3.x)*(p1.x - p3.x) + (p1.y - p3.y)*(p1.y - p3.y) + (p1.z - p3.z)*(p1.z - p3.z));
	float s = (a + b + c) / 2.;
	return (sqrt(s*((s - a)*(s - b)*(s - c))));
}
//distance of 2 point (with 3d pythagoras formula)
float Collision::pointdistace(vector3d p1, vector3d p2)
{
	vector3d vec(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	return (sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z));
}

float Collision::pointdistacesquare(vector3d p1, vector3d p2)
{
	vector3d vec(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	return (vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}


bool Collision::rayplane(const float& nx, float ny, float nz, float x0, float y0, float z0, float xs, float ys, float zs, float xd, float yd, float zd, vector3d p1, vector3d p2, vector3d p3, vector3d p4, float* dis, vector3d* point)
{
	if ((xd*nx + yd*ny + zd*nz) == 0)	//if the two vector dot product is 0, then there is no intersection (we don't like to divide by 0)
	{
		return false;
	}
	float t = ((x0*nx + y0*ny + z0*nz - nx*xs - ny*ys - nz*zs) / (xd*nx + yd*ny + zd*nz));
	if (t<0)	//if t<0, the intersction point is in the opposite direction
	{
		return false;
	}
	float x = xs + t*xd;	//calculate the 3 point vector3d
	float y = ys + t*yd;
	float z = zs + t*zd;
	vector3d i(x, y, z);

	if ((std::abs(trianglearea(p1, p2, p3) - (trianglearea(p1, p2, i) + trianglearea(p2, p3, i) + trianglearea(p1, p3, i)))<0.3) || std::abs(trianglearea(p1, p3, p4) - (trianglearea(p1, p3, i) + trianglearea(p3, p4, i) + trianglearea(p1, p4, i)))<0.3)	//we divide the quad to 2 triangle, we divide one triangle to 3 (one point is the
																																																															//intersection point), and if the area of the 3 triangle is equal to the main triangle, then the point is inside the triangle. We do the same with
																																																															//the other triangle, and if one is true, then the point is in the quad
	{
		if (dis != NULL)
		{
			(*dis) = t;
			if (point != NULL)
			{
				point->x = x;
				point->y = y;
				point->z = z;
			}
		}
		return true;
	}

	return false;	//else not
}

bool Collision::sphereplane(vector3d& sp, vector3d pn, vector3d p1, vector3d p2, vector3d p3, vector3d p4, float r)
{
	float dist1 = 0, dist2 = 0;

	if (rayplane(pn.x, pn.y, pn.z, p1.x, p1.y, p1.z, sp.x, sp.y, sp.z, -pn.x, -pn.y, -pn.z, p1, p2, p3, p4, &dist1) || rayplane(-pn.x, -pn.y, -pn.z, p1.x, p1.y, p1.z, sp.x, sp.y, sp.z, pn.x, pn.y, pn.z, p1, p2, p3, p4, &dist2))
	{
		if (dist1>r || dist2>r)
		{
			return false;
		}

		if (dist1>0)	//if not zero
		{

			// if plane normal y dimension is lower than 1, it means we go "upstairs", it means we have to raise y dimension value by small value so we are not 
			//forced to "stand in place"
			sp.x = pn.y > 0 && pn.y < 1 ? sp.x : sp.x + pn.x*(r - dist1);	//calculate the point, and calculate our new location
			sp.y = pn.y > 0 && pn.y < 1 ? sp.y + pn.y*(r - dist1) + 0.001 : sp.y + pn.y*(r - dist1);
			sp.z = pn.y > 0 && pn.y < 1 ? sp.z : sp.z + pn.z*(r - dist1);
		}
		else	//else if we are in the opposite side, then the normal vector
		{
			sp.x = sp.x - pn.x*(r - dist2);
			sp.y = sp.y - pn.y*(r - dist2);
			sp.z = sp.z - pn.z*(r - dist2);
		}

		return true;	//we return true, to indicate the collision, but the main thing is, that we changged the sp (so the sphere position) to the new position)
	}
	//	if(pn.x==0 && pn.y==1 && pn.z==0)
	//std::cout << "returning false (2): " << pn << dist1 << " " << dist2 << std::endl;
	return false;
}

bool Collision::spheresphere(vector3d& c1, float r1, vector3d& c2, float r2)
{
	float dist = pointdistacesquare(c1, c2);
	if (dist <= (r1 + r2)*(r1 + r2))
	{
		float a = sqrt(dist) - (r1 + r2);
		vector3d vec(c2 - c1);
		vec.normalize();
		c1 = c1 + vec*a;
		return 1;
	}
	return 0;
}

bool Collision::isCollision2points(float x, float z, vector2D vertices[3])
{
	int i;
	vector2D v1, v2;
	float resultY;

	for (i = 0; i < 3; i++)
	{
		v1.x = vertices[(i + 1) % 3].x - vertices[i].x;
		v1.z = vertices[(i + 1) % 3].z - vertices[i].z;
		v2.x = x - vertices[i].x;
		v2.z = z - vertices[i].z;

		resultY = v1.z*v2.x - v1.x*v2.z;
		if (resultY < 0.0f)
		{
			return 0;
		}
	}

	return 1;
}
