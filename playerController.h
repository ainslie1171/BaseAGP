#ifndef _PLAYERCONTROLLER_H_
#define _PLAYERCONTROLLER_H_

#include "inputManager.h"
#include "maths.h"
class Player;
class PlayerController
{
private:
	InputManager* m_inputManager;
	Vector3 m_rotation;
	Vector3 m_lookAt;
	Vector3 m_position;
	Player* m_player;

	float m_lookSensitivity;
	float m_movementSpeed;
public:
	PlayerController(InputManager* input, Player* player);
	~PlayerController();
	void Update(float deltaTime);
	Vector3 lookAt(){ return m_lookAt; }
	Vector3 Velocity(){ return m_position; }
	void setPosition(Vector3 Position) { m_position = Position; }
	void setSpeed(float speed) { m_movementSpeed = speed; }
};

#endif//_PLAYERCONTROLLER_H_

/*
Change Log:

*/