#include "Robot.h"
#include "stb_image.h"

Robot::Robot(float x, float y, float z, float rotX, float rotY, float rotZ)
{
	m_x = x;
	m_y = y;
	m_z = z;

	m_rotX = rotX;
	m_rotY = rotY;
	m_rotZ = rotZ;

	m_bodyW = 6.0f;
	m_bodyH = 6.0f;
	m_bodyD = 10.0f;

	m_cannonB = m_bodyW * 0.25f;
	m_cannonD = m_bodyD * 1.25f;

	m_hipsB = m_bodyW * 0.1f;
	m_hipsD = m_bodyW * 1.25f;
	m_hipsR = m_hipsB * 1.25f;

	m_upperLB = m_hipsR * 0.75f;
	m_upperLD = 3.0f;
	m_kneeR = m_hipsR;

	m_lowerLB = m_kneeR * 0.75f;
	m_lowerLT = m_kneeR * 2.25f;
	m_lowerLH = m_upperLD * 2.5f;

	m_footR = m_lowerLT;

	m_animationTimer = 0;
	m_currentState = init_state;

	m_velX = 0.0f;
	m_velY = 0.0f;
	m_velZ = 0.05f;

	m_LHipRot = 0.0f;
	m_LKneeRot = 0.0f;
	m_RHipRot = 0.0f;
	m_RKneeRot = 0.0f;
	m_cannonRot = 0.0f;

	m_randomRotY = 0.0f;

	m_active = true;
	m_boundingBox = new BoxCollider(m_bodyW, m_bodyH, m_bodyD);

	m_projectile.setDirection(90.0f);
	m_projectile.setVelZ(0.25f);

	m_right = true;

	glGenTextures(1, &m_textureid);
	glBindTexture(GL_TEXTURE_2D, m_textureid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texWidth, texHeight, texChannels;
	unsigned char* texture = stbi_load("robot.jpg", &texWidth, &texHeight, &texChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Robot::~Robot()
{
	delete m_boundingBox;
}


void Robot::drawProjectiles(BoxCollider& collider)
{

	if (m_projectile.active())
	{
		glPushMatrix();
			glTranslatef(m_x, m_y, m_z);
			m_projectile.draw();
			m_projectile.collisionCheck(collider);
		glPopMatrix();
	}
	else
	{
		if (m_active)
		{
			m_projectile.fire(0.0f, -m_bodyH * 0.5f, 0.0f, m_rotX, m_rotY + m_randomRotY, m_rotZ);
		}
	}
}

void Robot::draw()
{
	if (!m_active)
	{
		return;
	}

	if (m_z >= 50.0f)
	{
		m_active = false;
		return;
	}

	GLfloat robotMat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat robotMat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat robotMat_diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat robotMat_shininess[] = { 200.0f };

	GLfloat hitrobotMat_ambient[] = { 0.8f, 1.0f, 0.8f, 1.0f };
	GLfloat hitrobotMat_specular[] = { 0.4f, 1.0f, 0.4f, 1.0f };
	GLfloat hitrobotMat_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat hitrobotMat_shininess[] = { 200.0f };

	if (!m_boundingBox->active())
	{
		m_active = false;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, hitrobotMat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, hitrobotMat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, hitrobotMat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, hitrobotMat_shininess);
	}
	else
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, robotMat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, robotMat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, robotMat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, robotMat_shininess);
	}

	glBindTexture(GL_TEXTURE_2D, m_textureid);

	glPushMatrix();
		glTranslatef(m_x, m_y, m_z);
		glRotatef(m_randomRotY, 0.0f, 1.0f, 0.0f);
		glRotatef(m_rotX, 1.0f, 0.0f, 0.0f);
		glRotatef(m_rotY, 0.0f, 1.0f, 0.0f);
		glRotatef(m_rotZ, 0.0f, 0.0f, 1.0f);

		glPushMatrix();
			drawBody();
			glPushMatrix();
				drawHead();
				drawCannons();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0f, -0.0f, -m_bodyD * 0.35f);
				drawHips();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(m_x, m_y, m_z);
		glRotatef(m_randomRotY, 0.0f, 1.0f, 0.0f);
		glRotatef(m_rotX, 1.0f, 0.0f, 0.0f);
		glRotatef(m_rotY, 0.0f, 1.0f, 0.0f);
		glRotatef(m_rotZ, 0.0f, 0.0f, 1.0f);
		m_boundingBox->update();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Robot::drawHead()
{
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, m_bodyD * 0.5f);
		glutSolidCone(m_bodyW * 0.5, m_bodyH * 0.5, 32, 32);
	glPopMatrix();
}

void Robot::drawBody()
{
	glPushMatrix();
		glScalef(m_bodyW, m_bodyH, m_bodyD); 
		glutSolidCube(1.0);
	glPopMatrix();
}

void Robot::drawCannon()
{
	GLUquadricObj* cannon = gluNewQuadric();
	glPushMatrix();
		glRotatef(m_cannonRot, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, m_cannonD);
			glutSolidTorus(0.75f, 1.0f, 32, 32);
		glPopMatrix();
		gluCylinder(cannon, m_cannonB, m_cannonB, m_cannonD, 32, 32);
		glPushMatrix();
			glScalef(1.0f, 5.0f, 1.0f);
			glutSolidCube(1.0);
		glPopMatrix();
	glPopMatrix();
}

void Robot::drawCannons()
{
	glPushMatrix();
		glTranslatef(0.0f, m_bodyH * 0.5f, -m_bodyD * 0.75f);
		glPushMatrix();
			glTranslatef(-m_bodyW + m_cannonB, 0.0f, 0.0f);
			drawCannon();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0f, -m_bodyH, 0.0f);
			glScalef(0.5f, 0.5f, 1.0f);
			drawCannon();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(m_bodyW - m_cannonB, 0.0f, 0.0f);
			drawCannon();
		glPopMatrix();
	glPopMatrix();
}

void Robot::drawHips()
{
	glPushMatrix();
		glPushMatrix();
			glScalef(m_hipsD, m_hipsB, m_hipsB);
			glutSolidCube(1.0);
		glPopMatrix();

		glPushMatrix();
			glRotatef(m_LHipRot, -1.0f, 0.0f, 0.0f);
			glTranslatef(-m_hipsD / 2.0f, 0.0f, 0.0f);
			glRotatef(30.0f, 0.0f, -1.0f, 0.0f);
			glutSolidSphere(m_hipsR, 32, 32);
			glPushMatrix();
				drawUpperLeg();
				glPushMatrix();
					glTranslatef(0.0f, 0.0f, m_upperLD);
					glRotatef(m_LKneeRot, -1.0f, 0.0f, 0.0f);
					drawKnee();
						glPushMatrix();
							glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
							drawLowerLeg();
							glPushMatrix();
								drawFoot();
							glPopMatrix();
						glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glRotatef(m_RHipRot, -1.0f, 0.0f, 0.0f);
			glTranslatef(m_hipsD / 2.0f, 0.0f, 0.0f);
			glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
			glutSolidSphere(m_hipsR, 32, 32);
			glPushMatrix();
				drawUpperLeg();
				glPushMatrix();
					glTranslatef(0.0f, 0.0f, m_upperLD);
					glRotatef(m_RKneeRot, -1.0f, 0.0f, 0.0f);
					drawKnee();
					glPushMatrix();
						glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
						drawLowerLeg();
						glPushMatrix();
							drawFoot();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void Robot::drawUpperLeg()
{
	GLUquadricObj* upperLeg = gluNewQuadric();

	glPushMatrix();
		gluCylinder(upperLeg, m_upperLB, m_upperLB, m_upperLD, 32, 32);
	glPopMatrix();
}

void Robot::drawKnee()
{
	glPushMatrix();
		glutSolidSphere(m_kneeR, 32, 32);
	glPopMatrix();
}

void Robot::drawLowerLeg()
{
	GLUquadricObj* lowerLeg = gluNewQuadric();

	glPushMatrix();
		gluCylinder(lowerLeg, m_lowerLB, m_lowerLT, m_lowerLH, 32, 32);
	glPopMatrix();
}

void Robot::drawFoot()
{
	GLUquadricObj* foot = gluNewQuadric();

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, m_lowerLH);
		gluCylinder(foot, m_footR, m_footR, m_footR, 32, 32);
	glPopMatrix();
}

void Robot::animateCannons()
{
	m_cannonRot += 1.0f;
}

void Robot::animateWalk()
{
	if (m_right)
	{
		m_randomRotY += 0.35f;
		if (m_randomRotY >= 50.0f) m_right = false;
	}
	else
	{
		m_randomRotY -= 0.35f;
		if (m_randomRotY <= -50.0f) m_right = true;
	}

	if (glutGet(GLUT_ELAPSED_TIME) - m_animationTimer >= 1000)
	{
		m_currentState = (AnimationStates)((m_currentState + 1) % num_states);
		if (m_currentState == init_state) m_currentState = state0;
		m_animationTimer = glutGet(GLUT_ELAPSED_TIME);
	}

	switch (m_currentState)
	{
	case(init_state): 
		break;
	case(state0):
		m_LHipRot += 0.25f;
		m_LKneeRot += 0.25f;
		m_RHipRot -= 0.25f;
		m_RKneeRot -= 0.25f;

		break;
	case(state1):
		m_LHipRot -= 0.25f;
		m_LKneeRot -= 0.25f;
		m_RHipRot += 0.25f;
		m_RKneeRot += 0.25f;

		break;
	case(state2):
		m_LHipRot -= 0.25f;
		m_LKneeRot -= 0.25f;
		m_RHipRot += 0.25f;
		m_RKneeRot += 0.25f;

		break;
	case(state3):
		m_LHipRot += 0.25f;
		m_LKneeRot += 0.25f;
		m_RHipRot -= 0.25f;
		m_RKneeRot -= 0.25f;

		break;
	}

	m_z += m_velZ;
	m_x += m_velX;
	m_y += m_velY;
}

void Robot::animate()
{
	if (m_active)
	{
		animateWalk();
		animateCannons();
	}
}

BoxCollider& Robot::collider()
{
	return *m_boundingBox;
}

void Robot::reset(float x, float y, float z, float rotX, float rotY, float rotZ)
{
	m_x = x;
	m_y = y;
	m_z = z;

	m_rotX = rotX;
	m_rotY = rotY;
	m_rotZ = rotZ;

	m_LHipRot = 0.0f;
	m_LKneeRot = 0.0f;
	m_RHipRot = 0.0f;
	m_RKneeRot = 0.0f;
	m_cannonRot = 0.0f;
	m_randomRotY = 0.0f;

	m_animationTimer = glutGet(GLUT_ELAPSED_TIME);
	m_currentState = state0;

	m_velZ = 0.05f;
	m_active = true;
	m_boundingBox->active(true);
	m_projectile.active(false);
	m_right = !m_right;
}

bool Robot::active()
{
	return m_active;
}