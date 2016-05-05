#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "gameObject.h"
#include "bullet.h"

class Character : public GameObject
{
protected:
	int m_health;
	int m_maxHealth;
	float m_attackDelay;
	float m_attackTimer;
	bool m_canAttack;
	Vector3 m_eyePositionOffset;
	vector<Bullet*> m_pBullets;

	//Game* m_pGame;

	bool canAtack();

	//Particle system
	//vector<Bullet*> m_pBullets;
	void virtual Attack() = 0;
	void virtual Die() = 0;
	
public:
	Character(const GAMEOBJECT_DESC& desc);
	Character(const GAMEOBJECT_DESC& desc, int maxHealth);
	~Character(void);
	void virtual Update(void);
	void changeHealth();
	int getHealth() { return m_health; }
	void setAttackDelay(float attackDelay){ m_attackDelay = attackDelay; }
	virtual void Hit(int damage = 10);
	void detachBullets();
};

#endif//_CHARACTER_H_

/*
Change Log:
modified the constructor parameters to match the parent's
*/