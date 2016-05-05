#include "bullet.h"
#include "character.h"

Bullet::Bullet(Vector3 position, Vector3 velocity, float life, ParticleManager* particleManager, GameObject* parent, float scale)
{
	m_pParticle = particleManager->spawnParticleRef(position, velocity, life, parent, scale);
	m_pParent = parent;
}

Bullet::~Bullet()
{
	
}

void Bullet::Update(float deltaTime)
{
	//if (m_pParticle->dirty) this->~Bullet();
	GameObject* c = nullptr;
	if (m_pParticle->parent->getRoot()->checkCollisionRaySphereWRef(m_pParticle->position, m_pParticle->position+(m_pParticle->velocity * deltaTime), &c))
	{
		if (c != m_pParticle->parent && c != nullptr)
		{
			m_pParticle->dirty = true;
			if (dynamic_cast<Character*>(c) != 0)
				dynamic_cast<Character*>(c)->Hit();
		}
	}
}