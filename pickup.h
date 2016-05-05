#ifndef _PICKUP_H_
#define _PICKUP_H_

#include "particleManager.h"
#include "gameObject.h"



class Pickup
{
public:
	Pickup(Vector3 position, float life, ParticleManager* particleManager, GameObject* parent = nullptr, float scale = 1.0f);
	~Pickup();
	void Update(float deltaTime);
	bool isDirty(void){ return m_pParticle->dirty; }
	bool checkCollision(Pickup* p);
	Vector3 getPosition(){ return m_pParticle->position; }
	Vector3 getVelocity(){ return m_pParticle->velocity; }
	void setVelocity(Vector3 vel){ m_pParticle->velocity = vel; }
	float getScale(){ return m_pParticle->scale; }
	float getInvMass(){ return m_invMass; }
	void collisionResponse(Pickup* p);
	void doGravity(float deltaTime);
	
	enum ePickupType
	{
		ePT_Health, ePT_Ammo
	};

	ePickupType getType(){ return m_type; }
	void Dirty(){ m_pParticle->dirty = true; }

private:
	Particle* m_pParticle;
	GameObject* m_pPlayer;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_center;
	
	ePickupType m_type;

	float m_mass;
	float m_invMass;



};

#endif//_PICKUP_H_

/*
Change Log:

*/