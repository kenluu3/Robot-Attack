#pragma once

#define M_PI 3.14159265358979323846

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <algorithm>

class BoxCollider
{
private:
	float m_w, m_h, m_d;

	float m_minx, m_maxx;
	float m_miny, m_maxy;
	float m_minz, m_maxz;

	float m_vertex_0[3]; // Top Left (+Z Forward)
	float m_vertex_1[3]; // Top Right
	float m_vertex_2[3]; // Bottom Left
	float m_vertex_3[3]; // Bottom Right

	float m_vertex_4[3]; // Top Left (-Z Backward)
	float m_vertex_5[3]; // Top Right  
	float m_vertex_6[3]; // Bottom Left
	float m_vertex_7[3]; // Bottom Right

	float m_matrix[16]; // Transformation Matrix

	bool m_active;

public:
	BoxCollider(float w, float h, float d);
	~BoxCollider();

	bool collides(BoxCollider& other);
	void update();
	void active(bool status);
	bool active();

private:
	void computeBounds();
};