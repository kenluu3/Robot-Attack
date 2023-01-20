#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "BoxCollider.h"

#define M_PI 3.14159265358979323846

class Projectile
{
private:
	std::vector<GLdouble> m_vertices;
	std::vector<GLdouble> m_normals;
	std::vector<GLuint> m_indices;

	GLuint m_vertexid, m_normalsid, m_indicesid;

	float m_velX, m_velY, m_velZ;
	float m_rotX, m_rotY, m_rotZ;
	float m_x, m_y, m_z;
	float m_direction;

	bool m_active;

	BoxCollider* m_boundingBox;

public:
	Projectile();
	~Projectile();

	void draw();
	void collisionCheck(BoxCollider& collider);
	void fire(float x, float y, float z, float rotX, float rotY, float rotZ);
	bool active();
	void active(bool status);
	void setVelZ(float value);
	void setDirection(float angle);

	BoxCollider& collider();
};