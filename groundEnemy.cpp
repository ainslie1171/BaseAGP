#include "groundEnemy.h"

GroundEnemy::GroundEnemy(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player, PickupManager* pickupManager)
	:Enemy(desc, maxHealth, player, pickupManager)
{
	m_targetDirection.y = 0.0f;
	m_targetLocation.y = -1.0f;
	m_attackDelay = 1.0f;
	m_attackRange = 100.0f;
}

GroundEnemy::~GroundEnemy()
{

}

void GroundEnemy::Update()
{
	Enemy::Update();
	if (distanceBetweenVectorsSqr(m_Position, m_targetLocation) < 5.0f)
	{
		aquireNewTargetLocation();
		m_targetDirection.y = 0.0f;
		m_targetLocation.y = -1.0f;
	}
	
	Move(m_targetDirection*m_Speed*m_deltaTime);

	if (m_canAttack)
	{
		if (distanceBetweenVectorsSqr(m_Position, m_pPlayer->getTargetPosition()) < m_attackRange)
			Attack();
	}
}

void GroundEnemy::Die()
{
	m_pPickupManager->addCluster(m_Position+m_eyePositionOffset, rand() % 5 + 3);
	Remove();
}

void GroundEnemy::Attack(void)
{
	m_attackTimer = 0.0f;
	Vector3 direction = (m_pPlayer->getTargetPosition() - (m_Position + m_eyePositionOffset)).normalise();
	m_pBullets.push_back(new Bullet(m_Position + m_eyePositionOffset, direction * 10.0f, 2.0f, m_pParticleManager, this, 0.01f));
}

void GroundEnemy::setPosition(Vector3 position)
{
	m_Position = position;
	aquireNewTargetLocation();
	m_targetDirection.y = 0.0f;
	m_targetLocation.y = -1.0f;
}
