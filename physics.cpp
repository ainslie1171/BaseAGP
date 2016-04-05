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