#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

class Ground
{
private:
	GLuint m_textureid;

public:
	Ground();
	~Ground();

	void draw();
};