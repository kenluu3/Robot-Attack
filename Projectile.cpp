#include "Projectile.h"

Projectile::Projectile()
{
	std::ifstream infile("projectile.obj");

	if (infile.is_open())
	{
		std::string line;
		while (std::getline(infile, line))
		{
			std::stringstream stream(line);
			GLdouble x_vert, y_vert, z_vert;
			GLuint ind_1, ind_2, ind_3, ind_4;
			std::string token;

			while (stream >> token)
			{
				if (token == "f")
				{
					stream >> ind_1 >> ind_2 >> ind_3 >> ind_4;
					m_indices.push_back(ind_1);
					m_indices.push_back(ind_2);
					m_indices.push_back(ind_3);
					m_indices.push_back(ind_4);
				}
				else if (token == "vn")
				{
					stream >> x_vert >> y_vert >> z_vert;
					m_normals.push_back(x_vert);
					m_normals.push_back(y_vert);
					m_normals.push_back(z_vert);
				}
				else if (token == "v")
				{
					stream >> x_vert >> y_vert >> z_vert;
					m_vertices.push_back(x_vert);
					m_vertices.push_back(y_vert);
					m_vertices.push_back(z_vert);
				}
				continue;
			}
		}

		infile.close();
	}

	glGenBuffers(1, &m_vertexid);
	glGenBuffers(1, &m_normalsid);
	glGenBuffers(1, &m_indicesid);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexid);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLdouble), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalsid);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLdouble), &m_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_active = false;

	m_velX = 0.0f;
	m_velY = 0.0f;
	m_velZ = -0.5f;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;

	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;

	m_boundingBox = new BoxCollider(1.0f, 1.0f, 2.0f);

	m_direction = -90.0f;
}

Projectile::~Projectile()
{
	delete m_boundingBox;
}

bool Projectile::active()
{
	return m_active;
}

void Projectile::draw()
{
	if (m_active)
	{
		m_x += m_velX;
		m_y += m_velY;
		m_z += m_velZ;

		if (m_z <= -60.0f || m_z >= 60.0f)
		{
			m_active = false;
			return;
		}
		
		glPushMatrix();
			glRotatef(m_rotX, 1.0f, 0.0f, 0.0f);
			glRotatef(m_rotY, 0.0f, 1.0f, 0.0f);
			glRotatef(m_rotZ, 0.0f, 0.0f, 1.0f);
			glTranslatef(m_x, m_y, m_z);

			glPushMatrix();
				glPushMatrix();
					glRotatef(m_direction, 1.0f, 0.0f, 0.0f);
					glPushMatrix();
						glBindBuffer(GL_ARRAY_BUFFER, m_vertexid);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesid);

						glEnableClientState(GL_VERTEX_ARRAY);
						glEnableClientState(GL_NORMAL_ARRAY);

						glVertexPointer(3, GL_DOUBLE, 0, (void*)0);

						glBindBuffer(GL_ARRAY_BUFFER, m_normalsid);
						glNormalPointer(GL_DOUBLE, 0, (void*)0);

						glDrawElements(GL_QUADS, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, (void*)0);

						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

						glDisableClientState(GL_NORMAL_ARRAY);
						glDisableClientState(GL_VERTEX_ARRAY);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}
}

void Projectile::collisionCheck(BoxCollider& collider)
{
	glPushMatrix();
		glRotatef(m_rotX, 1.0f, 0.0f, 0.0f);
		glRotatef(m_rotY, 0.0f, 1.0f, 0.0f);
		glRotatef(m_rotZ, 0.0f, 0.0f, 1.0f);
		glTranslatef(m_x, m_y, m_z);
		m_boundingBox->update();
		if (collider.active())
			if (m_boundingBox->collides(collider)) collider.active(false);
	glPopMatrix();
}

void Projectile::fire(float x, float y, float z, float rotX, float rotY, float rotZ)
{
	m_active = true;
	m_x = x;
	m_y = y;
	m_z = z;
	m_rotX = rotX;
	m_rotY = rotY;
	m_rotZ = rotZ;
}

BoxCollider& Projectile::collider()
{
	return *m_boundingBox;
}

void Projectile::active(bool status)
{
	m_active = status;
}

void Projectile::setVelZ(float value)
{
	m_velZ = value;
}

void Projectile::setDirection(float angle)
{
	m_direction = angle;
}