#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "maths.h"
struct Particle;

struct p_Particle
{
	Vector3 P;
	Vector3 X;
	float M;
	float scale;
	float time;
	

	void update(float dt);
	void applyForce(const Vector3& A, float deltaTime);
	void stepPosition(float dt);
};

void applyImpulse(p_Particle& p, const Vector3& J);
void stepPosition(p_Particle& p, float deltaTime);
void applyForce(p_Particle& p, const Vector3& A, float deltaTime);

Vector3 ApplyForce(const Particle& p, const Vector3& f);
Vector3 CalcAcceleration(const Particle& p, const Vector3& f);

static const Vector3 gravity = { 0.0f, -9.81f, 0.0f };

#endif