#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"
#include "playerController.h"
#include "camera.h"
//#include "physics.h"

class Player : public Character
{
public:
	enum eMoveState
	{
		eMS_WALKING, eMS_FLYING, eMS_LANDING, eMS_LAUNCHING
	};

	Player(GAMEOBJECT_DESC desc, int maxHealth);
	~Player(void);
	void Update(void);
	void setPosition(Vector3 position);
	eMoveState getState() { return m_moveState; }
	float getSpeed() { return m_walkingSpeed; }
	void setSpeed(float speed);
	void Launch();
	void Land();
	void ChangeState();
	void Accelerate();
	void Decelerate();
	Vector3 getTargetPosition() { return m_Position + m_eyePositionOffset; }
	void virtual Attack();
	void virtual Die();
	void virtual Hit();

private:
	PlayerController* m_pController;
	Camera* m_pCamera;
	
	bool m_grounded;
	float m_walkingSpeed;
	float m_flyingSpeed;
	float m_minFlySpeed;
	float m_maxFlySpeed;

	eMoveState m_moveState;
};

#endif//_PLAYER_H_

/*
Change Log:

*/