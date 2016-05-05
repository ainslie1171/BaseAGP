#include "flyingEnemy.h"
#include "debugLine.h"

FlyingEnemy::FlyingEnemy(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player)
	:Enemy(desc, maxHealth, player)
{
	while (!validateTargetLocation())
	{
		aquireNewTargetLocation();
	}
	m_Speed = 15.0f;
	m_moveRange = 50.0f;
}

FlyingEnemy::~FlyingEnemy()
{

}

void FlyingEnemy::Update()
{
	//DebugLine::getInstance()->draw(m_Position, m_targetLocation);
	Enemy::Update();

	if (distanceBetweenVectorsSqr(m_Position, m_targetLocation) < 10.0f)
	{
		//OutputDebugString("Aquire new target\n\n");
		aquireNewTargetLocation();
		while (!validateTargetLocation())
		{
			aquireNewTargetLocation();
		}
	}
	Move(m_targetDirection * m_Speed * m_deltaTime);
}

void FlyingEnemy::setPosition(Vector3 position)
{
	m_Position = position;
	aquireNewTargetLocation();
	while (!validateTargetLocation())
	{
		aquireNewTargetLocation();
	}
}

void FlyingEnemy::Attack(void)
{
	
}

void FlyingEnemy::Die(void)
{

}

void FlyingEnemy::Hit()
{

}

bool FlyingEnemy::validateTargetLocation(void)
{

	if (m_targetLocation.x < -50.0f)
	{
		return false;
		//OutputDebugString("Direction Limit: -X\n\n");
	}
	else if (m_targetLocation.x > 50.0f)
	{
		return false;
		//OutputDebugString("Direction Limit: +X\n\n");
	}
	if (m_targetLocation.z < -50.0f)
	{
		return false;
		//OutputDebugString("Direction Limit: -Z\n\n");
	}
	else if (m_targetLocation.z > 50.0f)
	{
		return false;
		//OutputDebugString("Direction Limit: +Z\n\n");
	}

	if (m_targetLocation.y < 7.0f)
	{
		return false;
		//OutputDebugString("Direction Limit: -Y\n\n");
	}
	else if (m_targetLocation.y > 70.0f)
	{
		return false;
		//OutputDebugString("Direction Limit: +Y\n\n");
	}
	return true;
}