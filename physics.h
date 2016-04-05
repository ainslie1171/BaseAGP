#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "maths.h"
struct Particle;

Vector3 ApplyForce(const Particle& p, const Vector3& f);
Vector3 CalcAcceleration(const Particle& p, const Vector3& f);

static const Vector3 gravity = { 0.0f, -9.81f, 0.0f };

#endif