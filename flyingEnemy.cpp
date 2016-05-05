#include "flyingEnemy.h"
#include "debugLine.h"

FlyingEnemy::FlyingEnemy(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player, PickupManager* pickupManager)
	:Enemy(desc, maxHealth, player, pickupManager)
{
	while (!validateTargetLocation())
	{
		aquireNewTargetLocation();
	}
	m_Speed = 15.0f;
	m_moveRange = 50.0f;
	m_attackRange = 400.0f;
	m_attackDelay = 1.0f;

	//Turrets
	//Left
	GAMEOBJECT_DESC turretDesc = desc;
	turretDesc.model = desc.modelManager->getModel(1);

	Turret* t = new Turret(turretDesc, 30, player, pickupManager);
	addChild(t);
	t->setScale(0.1f);
	t->setPosition({ -1.0, 0.0f, 0.7f });
	m_leftTurrets.push_back(t);

	t = new Turret(turretDesc, 30, player, pickupManager);
	addChild(t);
	t->setScale(0.1f);
	t->setPosition({ -1.0, 0.0f, -0.7f });
	m_leftTurrets.push_back(t);

	//right
	t = new Turret(turretDesc, 30, player, pickupManager);
	addChild(t);
	t->setScale(0.1f);
	t->setPosition({ 1.0, 0.0f, 0.7f });
	m_rightTurrets.push_back(t);

	t = new Turret(turretDesc, 30, player, pickupManager);
	addChild(t);
	t->setScale(0.1f);
	t->setPosition({ 1.0, 0.0f, -0.7f });
	m_rightTurrets.push_back(t);
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

	//turret cleanup
	vector<Turret*>::iterator it = m_leftTurrets.begin();
	while (it != m_leftTurrets.end())
	{
		if (!(*it))
		{
			it = m_leftTurrets.erase(it);
		}
		else
		{
			it++;
		}
		if (m_leftTurrets.empty()) break;
	}

	it = m_rightTurrets.begin();
	while (it != m_rightTurrets.end())
	{
		if (!(*it))
		{
			it = m_rightTurrets.erase(it);
		}
		else
		{
			it++;
		}
		if (m_rightTurrets.empty()) break;
	}

	if (m_canAttack)
	{
		if (distanceBetweenVectorsSqr(m_Position, m_pPlayer->getTargetPosition()) < m_attackRange)
		{
			Attack();
			if (!m_leftTurrets.empty())
			{
				for (Turret* t : m_leftTurrets)
				{
					t->Update(true);
				}
			}

			if (!m_rightTurrets.empty())
			{
				for (Turret* t : m_rightTurrets)
				{
					t->Update(true);
				}
			}
		}
		else
		{
			for (Turret* t : m_leftTurrets)
			{
				t->Update(false);
			}
			for (Turret* t : m_rightTurrets)
			{
				t->Update(false);
			}
		}
	}
	else
	{
		for (Turret* t : m_leftTurrets)
		{
			t->Update(false);
		}
		for (Turret* t : m_rightTurrets)
		{
			t->Update(false);
		}
	}
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
	m_attackTimer = 0.0f;
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