#include "enemy.h"



Enemy::Enemy(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player)
	:Character(desc, maxHealth)
{
	m_pPlayer = player;
	m_moveRange = 10.0f;
	aquireNewTargetLocation();
	m_Speed = 1.0f;
}

Enemy::~Enemy()
{

}

void Enemy::Update(void)
{
	Character::Update();
}

void Enemy::aquireNewTargetLocation(void)
{
	m_targetDirection.x = float(rand() % 201 - 100) / 100.0f;
	m_targetDirection.y = float(rand() % 201 - 100) / 100.0f;
	m_targetDirection.z = float(rand() % 201 - 100) / 100.0f;
	m_targetDirection.normalise();
	m_targetLocation = m_Position + (m_targetDirection* m_moveRange);
}