#include "physics.h"
#include "particleGenerator.h"

Vector3 ApplyForce(const Particle& p, const Vector3& f)
{
	return Vector3(p.mass) * f;
}

Vector3 CalcAcceleration(const Particle& p, const Vector3& f)
{
	return f / Vector3(p.mass);
}

/*
void applyImpulse(p_Particle& p, const Vector3& J)
{
	p.Momentum += J;
}

void stepPosition(p_Particle& p, float deltaTime)
{
	p.Position += p.Momentum * deltaTime / p.Mass;
}

void applyForce(p_Particle& p, const Vector3& A, float deltaTime)
{
	p.Momentum += A * p.Mass * deltaTime;
}
*/


void p_Particle::update(float dt)
{
	applyForce(gravity, dt);
	stepPosition(dt);

	time += dt;
}

void p_Particle::applyForce(const Vector3& A, float dt)
{
	P += A * M * dt;
}

void p_Particle::stepPosition(float dt)
{
	X += P * dt / M;
}