#include "player.h"
#include "pickup.h"

Player::Player(GAMEOBJECT_DESC desc, int maxHealth)
	:Character(desc, maxHealth)
{
	
	m_pCamera = &Camera::getInstance();
	

	m_moveState = eMS_WALKING;
	m_walkingSpeed = 2.5f;
	m_flyingSpeed = 10.0f;
	m_minFlySpeed = 5.0f;
	m_maxFlySpeed = 30.0f;
	m_pController = new PlayerController(desc.inputManager, this);
	m_attackDelay = 0.2f;
	m_ammo = 30;
	m_maxAmmo = 100;
}

Player::~Player()
{
	if(m_pController) delete m_pController;
	//if (p) delete p;
}

void Player::Update()
{
	
	m_pController->Update(m_deltaTime);
	Character::Update();
	Vector3 difference = ZeroVector3;

	switch (m_moveState)
	{
	case eMS_WALKING:
		difference = m_pController->Velocity() - m_Position;
		difference.y += -9.81f * m_deltaTime;

		if (difference.y + m_Position.y < -1.0f)
		{
			difference.y = -1.0f - m_Position.y;
		}
		
		break;
	case eMS_LANDING:
		Land();
	case eMS_FLYING:

		difference = m_pController->lookAt().normalise();
		difference *= m_flyingSpeed * m_deltaTime;
		break;
	
	case eMS_LAUNCHING:
		Launch();
		break;
	}

	Move(difference);

	//Use position
	m_pController->setPosition(m_Position);
	//p->Position = m_Position;
	m_pCamera->setPosition(m_Position + m_eyePositionOffset);
	m_pCamera->setLookAt(m_pController->lookAt());
	
	
}

void Player::setPosition(Vector3 position)
{
	m_Position = position;
	m_pController->setPosition(position);
	m_pCamera->setPosition(position);
	//p->Position = position;
}

void Player::Launch()
{
	if (m_Position.y < 3.0f)
	{
		m_Position.y += 2.0f * m_deltaTime;
	}
	else
	{
		m_moveState = eMS_FLYING;
	}
}

void Player::Land()
{
	if (m_Position.y > -1.0f)
	{
		m_Position.y -= 2.0f * m_deltaTime;

		if (m_Position.y > 2.0f)
		{
			float deltaY = m_Position.y - 2.0f;
			m_flyingSpeed -= ((deltaY * deltaY)/2.0f) * m_deltaTime;
			if (m_flyingSpeed < 0.0f) m_flyingSpeed = 0.0f;
		}
		else
		{
			m_flyingSpeed = 0;
		}
	}
	else
	{
		m_moveState = eMS_WALKING;
	}
}

void Player::ChangeState()
{
	switch (m_moveState)
	{
	case eMS_WALKING:
	case eMS_LANDING:
		m_moveState = eMS_LAUNCHING;
		break;
	case eMS_FLYING:
	case eMS_LAUNCHING:
		m_moveState = eMS_LANDING;
		break;
	}
}

void Player::Accelerate()
{
	float delta = m_maxFlySpeed - m_flyingSpeed;
	m_flyingSpeed += delta * m_deltaTime;
}

void Player::Decelerate()
{
	float delta = m_minFlySpeed - m_flyingSpeed;
	m_flyingSpeed += delta * m_deltaTime;
}

void Player::Attack()
{
	if (!m_canAttack) return;
	m_attackTimer = 0.0f;
	Vector3 shootOffset = m_eyePositionOffset;
	shootOffset.y -= 0.1f;
	switch (m_moveState)
	{
	case eMS_WALKING:
		m_pBullets.push_back(new Bullet(m_Position + shootOffset, m_pController->lookAt().normalise() * 10.0f, 2.0f, m_pParticleManager, this, 0.01f));
		//m_pParticleManager->spawnParticle(m_Position + shootOffset, m_pController->lookAt().normalise() * 10.0f, 2.0f, nullptr, 0.01f);
		break;
	case eMS_FLYING:
		m_pBullets.push_back(new Bullet(m_Position + shootOffset, m_pController->lookAt().normalise() * (10.0f + m_flyingSpeed), 2.0f, m_pParticleManager, this, 0.01f));
		//m_pParticleManager->spawnParticle(m_Position + shootOffset, m_pController->lookAt().normalise() * (10.0f + m_flyingSpeed), 2.0f, this, 0.01f);
		break;
	}
}

void Player::Die()
{
	OutputDebugString("You Died!");
}

void Player::setSpeed(float speed)
{
	m_walkingSpeed = speed;
	m_pController->setSpeed(speed);
}

void Player::Hit(int damage)
{
	m_health -= damage;
	char outputString[50];
	sprintf_s(outputString, "Health: %d/%d\n\n", m_health, m_maxHealth);
	OutputDebugString(outputString);
	if (m_health <= 0)
		Die();
}

void Player::collectPickup(int type, int amount)
{
	OutputDebugString("Pickup Collected");

	switch (type)
	{
	case 0 :
		if (m_health + amount < m_maxHealth)
			m_health += amount;
		break;
	case 1:
		if (m_ammo + amount < m_maxAmmo)
			m_ammo += amount;
		break;
	}
}

bool Player::collidePickup(Pickup* p)
{
	float scaleOG = m_worldScale.x;
	m_worldScale.x *= 6.0f;
	bool result = checkCollisionRaySphere(p->getPosition(), (p->getPosition() + (p->getVelocity() * m_deltaTime)));
	m_worldScale.x = scaleOG;
	return result;
}