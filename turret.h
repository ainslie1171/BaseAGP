#ifndef _TURRET_H_
#define _TURRET_H_

#include "enemy.h"

class Turret : public Enemy
{
private:
	void Attack(void);

public:
	Turret(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player, PickupManager* pickupManager);
	~Turret();
	void Update(bool canAttack);
};

#endif//_TURRET_H_

/*
Change Log:

*/