#include "turret.h"

Turret::Turret(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player, PickupManager* pickupManager)
	:Enemy(desc, maxHealth, player, pickupManager)
{

}

Turret::~Turret()
{

}

void Turret::Update(bool canAttack)
{
	if (canAttack) Attack();
}

void Turret::Attack()
{
	Vector3 worldPos = m_pParent->getPosition() + m_Position;
	Vector3 direction = (m_pPlayer->getTargetPosition() - worldPos).normalise();
	//m_pBullets.push_back(new Bullet(worldPos, direction * 10.0f, 2.0f, m_pParticleManager, this, 0.01f));
}

