/*
CPS511 - Assignment 3: Robot Attack
Ken Luu (500901583)
*/

#define M_PI 3.14159265358979323846

#include <windows.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Ground.h"
#include "Robot.h"
#include "Cannon.h"

// Forward Declarations
void init();
void initShaders();
void display();
void reshape(int w, int h);
void keyboardHandler(unsigned char key, int x, int y);
void mousePassiveMotionHandler(int x, int y);
void animate(int param);
void resetGame();
std::string loadShader(const char* shader);

// Shaders
GLuint basevertexShaderID;
GLuint basefragmentShaderID;
GLuint baseprogramID;

GLuint texturevertexShaderID;
GLuint texturefragmentShaderID;
GLuint textureprogramID;

// Viewport
constexpr int viewportWidth{ 1080 }, viewportHeight{ 720 };

// Lighting
GLfloat lightPosition0[] = { 0.0f, 150.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat Kc = 1.0f;
GLfloat Kl = 0.0f;
GLfloat Kq = 0.0f;

// Colors
GLfloat friendlyMat_ambient[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat friendlyMat_specular[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat friendlyMat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat friendlyMat_shininess[] = { 200.0 };

GLfloat enemyMat_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat enemyMat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat enemyMat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat enemyMat_shininess[] = { 200.0 };

// Camera 
int mouseX, mouseY;
float pitch = 0.0f;
float yaw = -90.0f; // To look in -z direction

float eyeX{ 0.0f }, eyeY{ 0.0f }, eyeZ{ 18.0f };
float targetX{ 0.0f };
float targetY{ 0.0f };
float targetZ{ 1.0f };
float upX{ 0.0f }, upY{ 1.0f }, upZ{ 0.0f };

GLdouble fov{ 100.0 }, aspect{ (GLdouble) viewportWidth / viewportHeight }, znear{ 0.1 }, zfar{ 70 };

// Meshes
Ground* ground;
Cannon* cannon;
std::array<Robot*, 2> robots;

// Cannon
float t_cx{ 0.0f }, t_cy{ -3.00f }, t_cz{ 15.0f };

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(viewportWidth, viewportHeight);
	glutCreateWindow("ASSIGNMENT 3: ROBOT ATTACK");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardHandler);
	glutPassiveMotionFunc(mousePassiveMotionHandler);
	glutSetCursor(GLUT_CURSOR_NONE);

	glewInit();
	init();

	glutMainLoop();

	delete ground;
	delete cannon;

	return 0;
}

void init()
{
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glutWarpPointer(viewportWidth / 2, viewportHeight / 2);

	initShaders();

	mouseX = viewportWidth / 2;
	mouseY = viewportHeight / 2;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearDepth(1.0f);

	glViewport(0, 0, (GLsizei)viewportWidth, (GLsizei)viewportHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, znear, zfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ground = new Ground();
	cannon = new Cannon();

	robots[0] = new Robot(-35.0f, -1.0f, -35.0f, 0.0f, 20.0f, 0.0f);
	robots[1] = new Robot(35.0f, -1.0f, -40.0f, 0.0f, -20.0f, 0.0f);

	glutTimerFunc(0, animate, 0);
}

void initShaders()
{
	basevertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	basefragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderf = loadShader("basevertex.glsl");
	std::string fragmentShaderf = loadShader("basefragment.glsl");

	const char* vertexShader = vertexShaderf.c_str();
	const char* fragmentShader = fragmentShaderf.c_str();

	glShaderSource(basevertexShaderID, 1, &vertexShader, NULL);
	glShaderSource(basefragmentShaderID, 1, &fragmentShader, NULL);

	glCompileShader(basevertexShaderID);
	glCompileShader(basefragmentShaderID);

	baseprogramID = glCreateProgram();
	
	glAttachShader(baseprogramID, basevertexShaderID);
	glAttachShader(baseprogramID, basefragmentShaderID);

	glLinkProgram(baseprogramID);

	// Shaders for textures
	texturevertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	texturefragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string textureVertexShaderf = loadShader("texturevertex.glsl");
	std::string textureFragmentShaderf = loadShader("texturefragment.glsl");

	const char* textureVertexShader = textureVertexShaderf.c_str();
	const char* textureFragmentShader = textureFragmentShaderf.c_str();

	glShaderSource(texturevertexShaderID, 1, &textureVertexShader, NULL);
	glShaderSource(texturefragmentShaderID, 1, &textureFragmentShader, NULL);

	glCompileShader(texturevertexShaderID);
	glCompileShader(texturefragmentShaderID);

	textureprogramID = glCreateProgram();

	glAttachShader(textureprogramID, texturevertexShaderID);
	glAttachShader(textureprogramID, texturefragmentShaderID);

	glLinkProgram(textureprogramID);
}

std::string loadShader(const char* shader)
{
	std::ifstream infile(shader);
	if (infile.is_open())
	{
		std::stringstream buffer;
		buffer << infile.rdbuf();
		return buffer.str();
	}
}

void resetGame()
{
	glutWarpPointer(viewportWidth / 2, viewportHeight / 2);
	mouseX = viewportWidth / 2;
	mouseY = viewportHeight / 2;

	yaw = -90.0f;
	pitch = 0.0f;

	robots[0]->reset(-35.0f, -1.0f, -35.0f, 0.0f, 20.0f, 0.0f);
	robots[1]->reset(35.0f, -1.0f, -40.0f, 0.0f, -20.0f, 0.0f);
	cannon->reset();

	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, targetX + eyeX, targetY + eyeY, targetZ + eyeZ, upX, upY, upZ);

	glUseProgram(textureprogramID);
	
	glPushMatrix();
		glTranslatef(t_cx, t_cy, t_cz);
		if (cannon->active())
		{
			glRotatef(pitch, 1.0f, 0.0f, 0.0f);
			glRotatef(-(yaw + 90), 0.0f, 1.0f, 0.0f);
		}
		cannon->draw();
	glPopMatrix();

	glPushMatrix();
		for (auto& robot : robots)
			robot->draw();
	glPopMatrix();

	glPushMatrix();
		ground->draw();
	glPopMatrix();

	glUseProgram(baseprogramID);

	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, friendlyMat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, friendlyMat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, friendlyMat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, friendlyMat_shininess);

		glTranslatef(t_cx, t_cy, t_cz);
		cannon->drawProjectiles(robots);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, enemyMat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, enemyMat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, enemyMat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, enemyMat_shininess);

		for (auto& robot : robots)
			robot->drawProjectiles(cannon->collider());
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, targetX + eyeX, targetY + eyeY, targetZ + eyeZ, upX, upY, upZ);
}

void keyboardHandler(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'r':
	case 'R':
		resetGame();
		break;
	case ' ':
		cannon->fire(pitch, -(yaw + 90), 0.0f);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void mousePassiveMotionHandler(int x, int y)
{
	int dx = mouseX - x;
	int dy = mouseY - y;

	yaw -= (float)dx * 0.2f;
	pitch += (float)dy * 0.2f;

	if ((yaw + 90) >= 55.0f) yaw = (-90 + 55.0f);
	if ((yaw + 90) <= -55.0f) yaw = (-90 - 55.0f);

	if (pitch >= 25.0f) pitch = 25.0f;
	if (pitch <= 0.0f) pitch = 0.0f;

	mouseX = x;
	mouseY = y;

	targetX = cos(M_PI / 180 * pitch) * cos(M_PI / 180 * yaw);
	targetY = sin(M_PI / 180 * pitch);
	targetZ = cos(M_PI / 180 * pitch) * sin(M_PI / 180 * yaw);

	glutPostRedisplay();
}

void animate(int param)
{
	for (auto& robot : robots)
		robot->animate();

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, animate, 0);
}