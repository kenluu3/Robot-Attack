#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include "Projectile.h"
#include "Robot.h"

class Cannon
{
private:
	std::vector<GLdouble> m_vertices;
	std::vector<GLdouble> m_normals;
	std::vector<GLuint> m_indices;

	GLuint m_vertexid, m_normalsid, m_indicesid;
	GLuint m_textureid;

	std::array<Projectile, 10> m_projectiles;

	bool m_active;
	
	BoxCollider* m_boundingBox;

public:
	Cannon();
	~Cannon();

	void draw();
	void drawProjectiles(std::array<Robot*, 2> robots);
	void fire(float rotX, float rotY, float rotZ);
	bool active();
	void reset();

	BoxCollider& collider();
};