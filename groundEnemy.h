#ifndef _GROUNDENEMY_H_
#define _GROUNDENEMY_H_

#include "enemy.h"

class GroundEnemy : public Enemy
{
private:
	void Attack(void);
	void Die(void);
	void virtual Hit();
public:
	GroundEnemy(const GAMEOBJECT_DESC& desc, int maxHealth, Player* player);
	~GroundEnemy();
	void virtual Update(void);
	void virtual setPosition(Vector3 position);

};

#endif//_GROUNDENEMY_H_

/*
Change Log:

*/