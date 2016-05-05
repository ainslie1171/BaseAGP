#ifndef _BULLET_H_
#define _BULLET_H_

#include "particleManager.h"
#include "gameObject.h"

class Bullet
{
private:
	Particle* m_pParticle;
	GameObject* m_pParent;
public:
	Bullet(Vector3 position, Vector3 velocity, float life, ParticleManager* particleManager, GameObject* parent = nullptr, float scale = 1.0f);
	~Bullet();
	void Update(float deltaTime);
	bool isDirty(void){ return m_pParticle->dirty; }
};

#endif//_BULLET_H_

/*
Change Log:

*/