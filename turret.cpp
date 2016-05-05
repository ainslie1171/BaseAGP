#include "turret.h"

Turret::Turret(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player)
	:Enemy(desc, maxHealth, player)
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

}