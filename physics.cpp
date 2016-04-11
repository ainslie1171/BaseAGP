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
	checkColl = true;
}

bool p_Particle::simpleCollisionCheck(const p_Particle& p)
{
	float distanceSq = distanceBetweenVectorsSqr(X, p.X);

	float combRadiSq = pow((scale + p.scale), 2.0f);

	return (distanceSq <= combRadiSq);
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
		return (X.x < b.x);
	}
	else if (b.x > 0)
	{
		return (X.x > b.x);
	}

	if (b.y < 0)
	{
		return (X.y < b.y);
	}
	else if (b.y > 0)
	{
		return (X.y > b.y);
	}

	if (b.z < 0)
	{
		return (X.z < b.z);
	}
	else if (b.z > 0)
	{
		return (X.z > b.z);
	}

	return false;
}


void p_Particle::boundryCollisionResponse(const Vector3& b)
{
	Vector3 dif = b - X;
	if (b.x != 0)
		P.x = dif.x * M;
	if (b.y != 0)
		P.y = dif.y * M;
	if (b.z != 0)
		P.z = dif.z * M;
}


void p_Particle::collisionResponse(p_Particle& p)
{
	float x1, x2, eps;
	Vector3 v1, v2, v1x, v2x, v1y, v2y, dif(X - p.X);

	eps = 2.0f;
	dif = dif.normalise();

	v1 = P / M;
	x1 = dot(dif, v1);
	v1x = dif * x1;
	v1y = v1 - x1;


	dif *= -1;
	v2 = p.P / p.M;
	x2 = dot(dif, v2);
	v2x = dif * x2;
	v2y = v2 - v2x;


	P = Vector3(v1x*(M - p.M) / (M + p.M) + v2x*(eps * p.M) / (M + p.M) + v1y) * M;
	p.P = Vector3(v1x*(eps * M) / (M + p.M) + v2x*(p.M - M) / (M + p.M) + v2y) * p.M;
}