#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <cmath>

class vector3d {
public:
	float x, y, z;

	vector3d();
	vector3d(float a, float b);
	vector3d(float a, float b, float c);
	vector3d(const vector3d & a);
	~vector3d() {};
	float getX();
	float getY();
	float getZ();


	float dotproduct(const vector3d& vec2);
	vector3d crossproduct(const vector3d& vec2);
	float length();
	void change(float a, float b, float c);
	void change(vector3d&);
	void change(vector3d);
	void changeX(float a);
	void changeY(float b);
	void changeZ(float c);
	void normalize();
	static vector3d normalVector(const vector3d & vec1, const vector3d & vec2, const vector3d& vec3);
	bool operator==(const vector3d& vec2);
	bool operator!=(const vector3d& vec2);
	bool operator>=(const vector3d& vec2);
	vector3d operator+(const vector3d& vec2);
	vector3d operator+(const float vec2);
	vector3d operator-(const vector3d& vec2);
	vector3d operator*(const float& num);
	vector3d operator/(const float& num);
	vector3d& operator+=(const vector3d& vec2);
	vector3d& operator-=(const vector3d& vec2);
	vector3d& operator*=(const float& num);
	vector3d& operator/=(const float& num);
	friend std::ostream& operator<<(std::ostream& out, vector3d vec2);
};

class vector2D
{
public:
	float x, z;
	vector2D():x(0),z(0) {
	
	};
	vector2D(float x1, float z1) :x(x1), z(z1) {
	};

};
#endif
