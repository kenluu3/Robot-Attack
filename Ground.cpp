#include "Ground.h"
#include "stb_image.h"

Ground::Ground()
{
	glGenTextures(1, &m_textureid);
	glBindTexture(GL_TEXTURE_2D, m_textureid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texWidth, texHeight, texChannels;
	unsigned char* texture = stbi_load("ground.png", &texWidth, &texHeight, &texChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Ground::~Ground()
{

}

void Ground::draw()
{
	GLfloat groundMat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat groundMat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat groundMat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat groundMat_shininess[] = { 10.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, groundMat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, groundMat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, groundMat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, groundMat_shininess);

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_textureid);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3f(-150.0f, -10.0f, -50.0f);
			glVertex3f(-150.0f, -10.0f, 50.0f);
			glVertex3f(150.0f, -10.0f, 50.0f);
			glVertex3f(150.0f, -10.0f, -50.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}