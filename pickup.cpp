#include "pickup.h"

Pickup::Pickup(Vector3 position, float life, ParticleManager* particleManager, GameObject* parent, float scale)
{
	Vector3 velocity;
	velocity.x = float(rand() % 201 - 100) / 100.0f;
	velocity.y = float(rand() % 201 - 100) / 100.0f;
	velocity.z = float(rand() % 201 - 100) / 100.0f;

	m_center = position;

	position.x += float(rand() % 201 - 100) / 100.0f;
	position.y += float(rand() % 201 - 100) / 100.0f;
	position.z += float(rand() % 201 - 100) / 100.0f;

	velocity /= 8.0f;

	m_pParticle = particleManager->spawnParticleRef(position, velocity, life, parent, scale);
	
	m_type = ePickupType(rand() % 2);


	switch (m_type)
	{
	case ePT_Health:
		m_pParticle->colour = { 1.0f, 0.0f, 0.0f };
		break;
	case ePT_Ammo:
		m_pParticle->colour = { 0.0f, 0.0f, 1.0f };
		break;
	}

	m_mass = 10.0f;
	m_invMass = 1 / m_mass;
}

Pickup::~Pickup()
{

}

void Pickup::Update(float deltaTime)
{
	m_pParticle->position += m_pParticle->velocity * deltaTime;
}

void Pickup::doGravity(float deltaTime)
{
	Vector3 direction = (m_center - m_pParticle->position);
	direction /= m_pParticle->position.length();
	m_acceleration = direction / 1.0f;
	m_pParticle->velocity += (m_acceleration * deltaTime);
}


bool Pickup::checkCollision(Pickup* p)
{
	float distanceSq = distanceBetweenVectorsSqr(getPosition(), p->getPosition());

	float combRadiSq = pow((getScale() + p->getScale()), 2.0f);

	return (distanceSq <= combRadiSq);
}

void Pickup::collisionResponse(Pickup* p)
{
	Vector3 Velocity1 = getVelocity(), Velocity2 = p->getVelocity();

	Vector3 N = (getPosition() - p->getPosition());
	N = N.normalise();
	Vector3 Vr = (Velocity1 - Velocity2);
	Vector3 impulse = N * dot(Vr, N);

	Velocity1 -= impulse * m_invMass;
	Velocity2 += impulse * p->getInvMass();

	setVelocity(Velocity1);
	p->setVelocity(Velocity2);
}