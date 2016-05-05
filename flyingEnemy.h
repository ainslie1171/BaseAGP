#ifndef _FLYINGENEMY_H_
#define _FLYINGENEMY_H_

#include "enemy.h"
#include "turret.h"

class FlyingEnemy : public Enemy
{
private:
	vector<Turret*> m_leftTurrets;
	vector<Turret*> m_rightTurrets;

	void Attack(void);
	void Die(void);
	void Hit();
	bool validateTargetLocation(void);
public:
	FlyingEnemy(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player);
	~FlyingEnemy();
	void virtual Update(void);
	void virtual setPosition(Vector3 position);

};

#endif//_FLYINGENEMY_H_

/*
Change Log:

*/