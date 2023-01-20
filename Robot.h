#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <array>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include "Projectile.h"
#include "BoxCollider.h"

class Robot
{
private:
	/*
		Walking States:
			1. Left Leg Forward, Right Leg Back
			2. Left Leg to Start, Right Leg to Start
			3. Left Leg Back, Right Leg Forward
			4. Left Leg to Start, Right Leg to Start
	*/
	enum AnimationStates
	{
		init_state,
		state0,
		state1,
		state2,
		state3,

		num_states,
	};

	enum WalkingAnimationRotations
	{
		leftHip,
		leftKnee,
		rightHip,
		rightKnee,
	};

	float m_x, m_y, m_z;
	float m_rotX, m_rotY, m_rotZ;

	float m_bodyW, m_bodyH, m_bodyD;
	float m_cannonB, m_cannonD;
	float m_hipsB, m_hipsD, m_hipsR;
	float m_upperLB, m_upperLD, m_kneeR;
	float m_lowerLB, m_lowerLT, m_lowerLH;
	float m_footR;

	float m_cannonRot;
	float m_LHipRot, m_LKneeRot;
	float m_RHipRot, m_RKneeRot;

	float m_velX, m_velY, m_velZ;
	
	bool m_active;
	float m_randomRotY;

	bool m_right;

	int m_animationTimer;
	AnimationStates m_currentState;

	Projectile m_projectile;

	BoxCollider* m_boundingBox;
	GLuint m_textureid;

public:
	Robot(float x, float y, float z, float rotX, float rotY, float rotZ);
	~Robot();

	void draw();
	void drawProjectiles(BoxCollider& collider);
	void animate();
	void reset(float x, float y, float z, float rotX, float rotY, float rotZ);
	bool active();

	BoxCollider& collider();

private:
	void drawBody();
	void drawHead();
	void drawCannon();
	void drawCannons();
	void drawHips();
	void drawUpperLeg();
	void drawLowerLeg();
	void drawKnee();
	void drawFoot();

	void animateWalk();
	void animateCannons();
};