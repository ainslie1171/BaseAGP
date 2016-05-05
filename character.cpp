#include "character.h"


Character::Character(const GAMEOBJECT_DESC& desc)
: GameObject(desc)
{
	m_attackTimer = 0.0f;
}

Character::Character(const GAMEOBJECT_DESC& desc, int maxHealth)
: GameObject(desc)
{
	m_maxHealth = maxHealth;
	m_health = maxHealth;
	m_attackTimer = 0.0f;
	m_eyePositionOffset = { 0.0f, 0.7f, 0.0f };
}

Character::~Character()
{

}

void Character::Update()
{
	for (Bullet* b : m_pBullets)
	{
		b->Update(m_deltaTime);
	}
	detachBullets();

	m_attackTimer += m_deltaTime;
	m_canAttack = (m_attackTimer > m_attackDelay);
}

void Character::changeHealth()
{

}

bool Character::canAtack()
{
	return m_canAttack;
}


void Character::detachBullets()
{
	vector<Bullet*>::iterator it = m_pBullets.begin();
	while(it != m_pBullets.end())
	{
		if ((*it)->isDirty())
		{
			delete (*it);
			it = m_pBullets.erase(it);
		}
		else
		{
			it++;
		}

		if (m_pBullets.empty()) break;
		
	}
	
}