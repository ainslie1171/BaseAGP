#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "player.h"

class Enemy:public Character
{
protected:
	Vector3 m_targetLocation;
	Vector3 m_targetDirection;
	Player* m_pPlayer;
	float m_moveRange;
	//Note this value should be squared
	float m_attackRange;
	void aquireNewTargetLocation(void);
	virtual void Die(void) = 0;
	virtual void Attack(void) = 0;
	void virtual Hit() = 0;
public:
	Enemy(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player);
	~Enemy();
	void virtual Update(void);
	
};

#endif//_ENEMY_H_

/*
Change Log:

*/