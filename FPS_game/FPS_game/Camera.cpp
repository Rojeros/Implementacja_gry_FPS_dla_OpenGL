#include <stdio.h>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

// SDL2 Headers
#include <SDL.h>
#include "camera.h"
#include <cmath>
#include "vector.h"
#define M_PI_2 3.14159265358979323846
#define M_PI 1.57079632679489661923

void Camera::init()
{
	m_yaw = 0.0;
	m_pitch = 0.0;

	location = vector3d(2, 1, 2);
}

void Camera::refresh()
{

	// Camera parameter according to Riegl's co-ordinate system
	m_lx = cos(m_yaw) * cos(m_pitch);
	m_ly = sin(m_pitch);
	m_lz = sin(m_yaw) * cos(m_pitch);

	m_strafe_lx = cos(m_yaw - M_PI_2);
	m_strafe_lz = sin(m_yaw - M_PI_2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(location.x, location.y, location.z, location.x + m_lx, location.y + m_ly, location.z + m_lz, 0.0, 1.0, 0.0);

}



void Camera::setLocation(const vector3d& newLocation)
{
	location = newLocation;

	refresh();
}


vector3d Camera::getLocation()
{
	return location;
}

void Camera::getDirectionVector(float &x, float &y, float &z)
{
	x = m_lx;
	y = m_ly;
	z = m_lz;
}
vector3d Camera::getDirectionVector()
{
	return(vector3d(m_lx, m_ly, m_lz));

}
//do porzodu lub do ty³u
void Camera::move(float incr)
{

	float lx = cos(m_yaw)*cos(m_pitch);
	float lz = sin(m_yaw)*cos(m_pitch);

	location.x +=  incr*lx;
	location.z +=  incr*lz;
	refresh();
}
//na boki
void Camera::strafe(float incr)
{
	float lx = cos(m_yaw)*cos(m_pitch);
	float lz = sin(m_yaw)*cos(m_pitch);

	location.x += -lz*incr;
	location.z += lx*incr;
	refresh();
}

void Camera::fly(float incr)
{
	location.y = location.y + incr;
	refresh();
}

void Camera::rotateYaw(float angle)
{
	m_yaw += angle;

	refresh();
}

void Camera::rotatePitch(float angle)
{
	const float limit = 89.0 * M_PI / 180.0;

	m_pitch += angle;

	if (m_pitch < -limit)
		m_pitch = -limit;

	if (m_pitch > limit)
		m_pitch = limit;

	refresh();
}

void Camera::setYaw(float angle)
{
	m_yaw = angle;

	refresh();
}

void Camera::setPitch(float angle)
{
	m_pitch = angle;

	refresh();
}

float Camera::getYaw() {
	return m_yaw;
}

float Camera::getPitch() {
	return m_pitch;
}