#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "maths.h"
struct Particle;

struct p_Particle
{
	Vector3 Position;
	Vector3 Velocity;
	float Mass;
	float InvMass;
	float scale;
	float time;
	Vector4 c;

	bool checkColl;

	void update(float dt);
	void applyForce(const Vector3& A, float deltaTime);
	void stepPosition(float dt);
	bool simpleCollisionCheck(const p_Particle& p);
	bool betterCollisionCheck(const p_Particle& p, float dt);
	void collisionResponse(p_Particle& p);
	void simpleCheckBoundries();
	void betterCheckBoundries(float dt);
	bool simpleBoundryCheck(const Vector3& b);
	bool betterBoundryCheck(const Vector3& b, float dt);
	void boundryCollisionResponse(const Vector3& b);
	void altBoundryCollisionResponse(const Vector3& b);
};

void applyImpulse(p_Particle& p, const Vector3& J);
void stepPosition(p_Particle& p, float deltaTime);
void applyForce(p_Particle& p, const Vector3& A, float deltaTime);

Vector3 ApplyForce(const Particle& p, const Vector3& f);
Vector3 CalcAcceleration(const Particle& p, const Vector3& f);

static const Vector3 gravity = { 0.0f, -9.81f, 0.0f };

#endif