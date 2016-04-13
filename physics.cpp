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
	Velocity += A * dt;
}

void p_Particle::stepPosition(float dt)
{
	Position += Velocity * dt;
	checkColl = true;
}

bool p_Particle::simpleCollisionCheck(const p_Particle& p)
{
	float distanceSq = distanceBetweenVectorsSqr(Position, p.Position);

	float combRadiSq = pow((scale + p.scale), 2.0f);

	if (distanceSq <= combRadiSq)
	{
		c = { 1.0f, 0.0f, 0.0f, 1.0f };
		return true;
	}
	else
	{
		c = { 0.0f, 0.5f, 0.5f, 1.0f };
		return false;
	}
}

bool p_Particle::betterCollisionCheck(const p_Particle& p, float dt)
{
	Vector3 ParticleA = Position + (Velocity*dt);
	Vector3 ParticleB = p.Position;
	ParticleB += (p.Velocity * dt);

	float distanceSq = distanceBetweenVectorsSqr(ParticleA, ParticleB);

	float combRadiSq = pow((scale + p.scale), 2.0f);

	if (distanceSq <= combRadiSq)
	{
		c = { 1.0f, 0.0f, 0.0f, 1.0f };
		return true;
	}
	else
	{
		c = { 0.0f, 0.5f, 0.5f, 1.0f };
		return false;
	}
}

void p_Particle::checkBoundries()
{
	Vector3 bot, left, right, front, back;
	bot = { 0.0f, -5.0f, 0.0f };
	left = { -5.0f, 0.0f, 0.0f };
	right = { 5.0f, 0.0f, 0.0f };
	front = { 0.0f, 0.0f, -5.0f };
	back = { 0.0f, 0.0f, 5.0f };



	if (simpleBoundryCheck(bot))
		boundryCollisionResponse(bot);

	if (simpleBoundryCheck(left))
		boundryCollisionResponse(left);

	if (simpleBoundryCheck(right))
		boundryCollisionResponse(right);

	if (simpleBoundryCheck(front))
		boundryCollisionResponse(front);

	if (simpleBoundryCheck(back))
		boundryCollisionResponse(back);
}

bool p_Particle::simpleBoundryCheck(const Vector3& b)
{
	if (b.x < 0)
	{
		return (Position.x < b.x);
	}
	else if (b.x > 0)
	{
		return (Position.x > b.x);
	}

	if (b.y < 0)
	{
		return (Position.y < b.y);
	}
	else if (b.y > 0)
	{
		return (Position.y > b.y);
	}

	if (b.z < 0)
	{
		return (Position.z < b.z);
	}
	else if (b.z > 0)
	{
		return (Position.z > b.z);
	}

	return false;
}


void p_Particle::boundryCollisionResponse(const Vector3& b)
{
	Vector3 dif = b - Position;
	if (b.x != 0)
		Velocity.x = dif.x;
	if (b.y != 0)
		Velocity.y = dif.y;
	if (b.z != 0)
		Velocity.z = dif.z;
}


void p_Particle::collisionResponse(p_Particle& p)
{
	Vector3 N = (Position - p.Position);
	N = N.normalise();
	Vector3 Vr = (Velocity - p.Velocity);
	Vector3 impulse =  N * dot(Vr, N);

	Velocity -= impulse * InvMass;
	p.Velocity += impulse * p.InvMass;
}