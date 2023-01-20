#include "Cannon.h"
#include "stb_image.h"

Cannon::Cannon()
{
	std::ifstream infile("cannon.obj");

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

	m_active = true;
	m_boundingBox = new BoxCollider(3.0f, 3.0f, 3.0f);
	
	glGenTextures(1, &m_textureid);
	glBindTexture(GL_TEXTURE_2D, m_textureid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texWidth, texHeight, texChannels;
	unsigned char* texture = stbi_load("cannon.jpg", &texWidth, &texHeight, &texChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Cannon::~Cannon()
{
	delete m_boundingBox;
}

void Cannon::draw()
{
	if (!m_boundingBox->active())
	{
		m_active = false;
	}

	GLfloat cannonMat_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat cannonMat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat cannonMat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat cannonMat_shininess[] = { 20.0f };

	GLfloat deadcannonMat_ambient[] = { 1.0f, 0.8f, 0.8f, 1.0f };
	GLfloat deadcannonMat_specular[] = { 1.0f, 0.4f, 0.4f, 1.0f };
	GLfloat deadcannonMat_diffuse[] = { 1.0f, 0.4f, 0.4f, 1.0f };
	GLfloat deadcannonMat_shininess[] = { 20.0f };

	if (!m_active)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, deadcannonMat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, deadcannonMat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, deadcannonMat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, deadcannonMat_shininess);
	}
	else
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cannonMat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cannonMat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cannonMat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cannonMat_shininess);
	}

	glBindTexture(GL_TEXTURE_2D, m_textureid);

	glPushMatrix();
		if (m_active)
	 		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		else
			glRotatef(-100.0f, 1.0f, 0.0f, 0.0f);

		glPushMatrix();
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexid);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesid);
			glBindTexture(GL_TEXTURE_2D, m_textureid);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_DOUBLE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, m_normalsid);
			glNormalPointer(GL_DOUBLE, 0, (void*)0);

			glDrawElements(GL_QUADS, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
		m_boundingBox->update();
	glPopMatrix();
}


void Cannon::drawProjectiles(std::array<Robot*, 2> robots)
{
	for (auto& projectile : m_projectiles)
	{
		if (projectile.active())
		{
			projectile.draw();
			for (auto robot : robots)
			{
				if (robot->active())
					projectile.collisionCheck(robot->collider());
			}
		}
	}
}


void Cannon::fire(float rotX, float rotY, float rotZ)
{
	if (!m_active)
	{
		return;
	}

	for (auto& projectile : m_projectiles)
	{
		if (!projectile.active())
		{
			projectile.fire(0.0f, 0.0f, 0.0f, rotX, rotY, rotZ);
			break;
		}
	}
}	

bool Cannon::active()
{
	return m_active;
}

void Cannon::reset()
{
	for (auto& projectile : m_projectiles)
	{
		projectile.active(false);
	}

	m_active = true;
	m_boundingBox->active(true);
}

BoxCollider& Cannon::collider()
{
	return *m_boundingBox;
}