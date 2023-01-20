#include "BoxCollider.h"

BoxCollider::BoxCollider(float w, float h, float d)
	: m_matrix{}
{
	m_w = w;
	m_h = h;
	m_d = d;

	m_minx = 0.0f;
	m_maxx = 0.0f;
	m_miny = 0.0f;
	m_maxy = 0.0f;
	m_minz = 0.0f;
	m_maxz = 0.0f;

	// +Z
	m_vertex_0[0] = -m_w / 2.0f;
	m_vertex_0[1] = m_h / 2.0f;
	m_vertex_0[2] = m_d / 2.0f;

	m_vertex_1[0] = m_w / 2.0f;
	m_vertex_1[1] = m_h / 2.0f;
	m_vertex_1[2] = m_d / 2.0f;

	m_vertex_2[0] = -m_w / 2.0f;
	m_vertex_2[1] = -m_h / 2.0f;
	m_vertex_2[2] = m_d / 2.0f;

	m_vertex_3[0] = m_w / 2.0f;
	m_vertex_3[1] = -m_h / 2.0f;
	m_vertex_3[2] = m_d / 2.0f;

	// -Z
	m_vertex_4[0] = -m_w / 2.0f;
	m_vertex_4[1] = m_h / 2.0f;
	m_vertex_4[2] = -m_d / 2.0f;

	m_vertex_5[0] = m_w / 2.0f;
	m_vertex_5[1] = m_h / 2.0f;
	m_vertex_5[2] = -m_d / 2.0f;

	m_vertex_6[0] = -m_w / 2.0f;
	m_vertex_6[1] = -m_h / 2.0f;
	m_vertex_6[2] = -m_d / 2.0f;

	m_vertex_7[0] = m_w / 2.0f;
	m_vertex_7[1] = -m_h / 2.0f;
	m_vertex_7[2] = -m_d / 2.0f;

	m_active = true;
}

BoxCollider::~BoxCollider()
{

}

bool BoxCollider::collides(BoxCollider& other)
{
	other.computeBounds();
	computeBounds();
	return m_minx <= other.m_maxx && m_maxx >= other.m_minx && m_miny <= other.m_maxy && m_maxy >= other.m_miny && m_minz <= other.m_maxz && m_maxz >= other.m_minz;
}

void BoxCollider::update()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);

	/* Draws Collision Detection Bounding Box
	GLfloat quadMat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat quadMat_specular[] = { 0.45f, 0.55f, 0.45f, 1.0f };
	GLfloat quadMat_diffuse[] = { 0.1f, 0.35f, 0.1f, 1.0f };
	GLfloat quadMat_shininess[] = { 10.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, quadMat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, quadMat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, quadMat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, quadMat_shininess);

	glPushMatrix();
	glScalef(m_w, m_h, m_d);
	glutSolidCube(1.0f);
	glPopMatrix();
	*/
}

void BoxCollider::computeBounds()
{
	/*
	[   
		 X    Y    Z    T
		 0    1    2    3
		m_0, m_4, m_8,  m_12,
		 4    5    6     7
		m_1, m_5, m_9,  m_13,
		 8    9    10    11
		m_2, m_6, m_10, m_14,
		 12   13   14    15
		m_3, m_7, m_11, m_15,
	]
	*/
	float v0_x = m_matrix[0] * m_vertex_0[0] + m_matrix[4] * m_vertex_0[1] + m_matrix[8] * m_vertex_0[2] + m_matrix[12];
	float v1_x = m_matrix[0] * m_vertex_1[0] + m_matrix[4] * m_vertex_1[1] + m_matrix[8] * m_vertex_1[2] + m_matrix[12];
	float v2_x = m_matrix[0] * m_vertex_2[0] + m_matrix[4] * m_vertex_2[1] + m_matrix[8] * m_vertex_2[2] + m_matrix[12];
	float v3_x = m_matrix[0] * m_vertex_3[0] + m_matrix[4] * m_vertex_3[1] + m_matrix[8] * m_vertex_3[2] + m_matrix[12];
	float v4_x = m_matrix[0] * m_vertex_4[0] + m_matrix[4] * m_vertex_4[1] + m_matrix[8] * m_vertex_4[2] + m_matrix[12];
	float v5_x = m_matrix[0] * m_vertex_5[0] + m_matrix[4] * m_vertex_5[1] + m_matrix[8] * m_vertex_5[2] + m_matrix[12];
	float v6_x = m_matrix[0] * m_vertex_6[0] + m_matrix[4] * m_vertex_6[1] + m_matrix[8] * m_vertex_6[2] + m_matrix[12];
	float v7_x = m_matrix[0] * m_vertex_7[0] + m_matrix[4] * m_vertex_7[1] + m_matrix[8] * m_vertex_7[2] + m_matrix[12];

	float v0_y = m_matrix[1] * m_vertex_0[0] + m_matrix[5] * m_vertex_0[1] + m_matrix[9] * m_vertex_0[2] + m_matrix[13];
	float v1_y = m_matrix[1] * m_vertex_1[0] + m_matrix[5] * m_vertex_1[1] + m_matrix[9] * m_vertex_1[2] + m_matrix[13];
	float v2_y = m_matrix[1] * m_vertex_2[0] + m_matrix[5] * m_vertex_2[1] + m_matrix[9] * m_vertex_2[2] + m_matrix[13];
	float v3_y = m_matrix[1] * m_vertex_3[0] + m_matrix[5] * m_vertex_3[1] + m_matrix[9] * m_vertex_3[2] + m_matrix[13];
	float v4_y = m_matrix[1] * m_vertex_4[0] + m_matrix[5] * m_vertex_4[1] + m_matrix[9] * m_vertex_4[2] + m_matrix[13];
	float v5_y = m_matrix[1] * m_vertex_5[0] + m_matrix[5] * m_vertex_5[1] + m_matrix[9] * m_vertex_5[2] + m_matrix[13];
	float v6_y = m_matrix[1] * m_vertex_6[0] + m_matrix[5] * m_vertex_6[1] + m_matrix[9] * m_vertex_6[2] + m_matrix[13];
	float v7_y = m_matrix[1] * m_vertex_7[0] + m_matrix[5] * m_vertex_7[1] + m_matrix[9] * m_vertex_7[2] + m_matrix[13];

	float v0_z = m_matrix[2] * m_vertex_0[0] + m_matrix[6] * m_vertex_0[1] + m_matrix[10] * m_vertex_0[2] + m_matrix[14];
	float v1_z = m_matrix[2] * m_vertex_1[0] + m_matrix[6] * m_vertex_1[1] + m_matrix[10] * m_vertex_1[2] + m_matrix[14];
	float v2_z = m_matrix[2] * m_vertex_2[0] + m_matrix[6] * m_vertex_2[1] + m_matrix[10] * m_vertex_2[2] + m_matrix[14];
	float v3_z = m_matrix[2] * m_vertex_3[0] + m_matrix[6] * m_vertex_3[1] + m_matrix[10] * m_vertex_3[2] + m_matrix[14];
	float v4_z = m_matrix[2] * m_vertex_4[0] + m_matrix[6] * m_vertex_4[1] + m_matrix[10] * m_vertex_4[2] + m_matrix[14];
	float v5_z = m_matrix[2] * m_vertex_5[0] + m_matrix[6] * m_vertex_5[1] + m_matrix[10] * m_vertex_5[2] + m_matrix[14];
	float v6_z = m_matrix[2] * m_vertex_6[0] + m_matrix[6] * m_vertex_6[1] + m_matrix[10] * m_vertex_6[2] + m_matrix[14];
	float v7_z = m_matrix[2] * m_vertex_7[0] + m_matrix[4] * m_vertex_7[1] + m_matrix[10] * m_vertex_7[2] + m_matrix[14];

	m_minx = std::min({ v0_x, v1_x, v2_x, v3_x, v4_x, v5_x, v6_x, v7_x });
	m_maxx = std::max({ v0_x, v1_x, v2_x, v3_x, v4_x, v5_x, v6_x, v7_x });

	m_miny = std::min({ v0_y, v1_y, v2_y, v3_y, v4_y, v5_y, v6_y, v7_y });
	m_maxy = std::max({ v0_y, v1_y, v2_y, v3_y, v4_y, v5_y, v6_y, v7_y });

	m_minz = std::min({ v0_z, v1_z, v2_z, v3_z, v4_z, v5_z, v6_z, v7_z });
	m_maxz = std::max({ v0_z, v1_z, v2_z, v3_z, v4_z, v5_z, v6_z, v7_z });
}

bool BoxCollider::active()
{
	return m_active;
}

void BoxCollider::active(bool status)
{
	m_active = status;
}