#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "vector.h"


class Camera
{

public:
	Camera() { init(); }
	~Camera() {}

	void init();

	void refresh();


	//void setPosition(float x, float y, float z);
	void setLocation(const vector3d& newPosition);
	void getDirectionVector(float &x, float &y, float &z);
	vector3d getDirectionVector();
	void setYaw(float angle);
	void setPitch(float angle);
	vector3d getLocation();
	void move(float incr);
	void strafe(float incr);
	void fly(float incr);
	void rotateYaw(float angle);
	void rotatePitch(float angle);
	float getYaw();
	float getPitch();

private:


	float m_lx, m_ly, m_lz; // Direction vector of where we are looking at
	float m_yaw, m_pitch; // Various rotation angles
	float m_strafe_lx, m_strafe_lz; // Always 90 degree to direction vector
	vector3d location;
};

#endif
// End of camera.h
