#include "playerController.h"
#include "player.h"

PlayerController::PlayerController(InputManager* input, Player* player)
{
	m_inputManager = input;
	m_lookAt = ZeroVector3;
	m_rotation = ZeroVector3;
	m_position = ZeroVector3;
	m_lookSensitivity = 180.0f;
	m_movementSpeed = player->getSpeed();
	m_player = player;
}

PlayerController::~PlayerController()
{

}

void PlayerController::Update(float deltaTime)
{
	if (m_player->getState() == Player::eMS_WALKING)
	{
		float speed = deltaTime * m_movementSpeed;

		if (m_inputManager->IsKeyPressed(DIK_W))
		{
			m_position.x += m_lookAt.x * speed;
			m_position.z += m_lookAt.z * speed;
		}
		else if (m_inputManager->IsKeyPressed(DIK_S))
		{
			m_position.x -= m_lookAt.x * speed;
			m_position.z -= m_lookAt.z * speed;
		}
		if (m_inputManager->IsKeyPressed(DIK_A))
		{
			m_position.x -= sin((m_rotation.x + 90) * (XM_PI / 180.0f)) * speed;
			m_position.z -= cos((m_rotation.x + 90) * (XM_PI / 180.0f)) * speed;
		}
		else if (m_inputManager->IsKeyPressed(DIK_D))
		{
			m_position.x += sin((m_rotation.x + 90) * (XM_PI / 180.0f)) * speed;
			m_position.z += cos((m_rotation.x + 90) * (XM_PI / 180.0f)) * speed;
		}
		if (m_inputManager->IsKeyPressed(DIK_E))
		{
			m_position.y += speed;
		}
		else if (m_inputManager->IsKeyPressed(DIK_Q))
		{
			m_position.y -= speed;
		}
	}
	else if (m_player->getState() == Player::eMS_FLYING)
	{
		if (m_inputManager->IsKeyPressed(DIK_W))
		{
			m_player->Accelerate();
		}
		else if (m_inputManager->IsKeyPressed(DIK_S))
		{
			m_player->Decelerate();
		}
	}


	m_rotation.x += m_inputManager->getMouseData().x * m_lookSensitivity * deltaTime;
	m_lookAt.x = sin(m_rotation.x * (XM_PI / 180.0f));
	m_lookAt.z = cos(m_rotation.x * (XM_PI / 180.0f));

	float delatY = m_inputManager->getMouseData().y * m_lookSensitivity * -deltaTime;
	if (m_rotation.y + delatY < 90 && m_rotation.y + delatY > -90)
	{
		m_rotation.y += delatY;
	}
	m_lookAt.y = sin(m_rotation.y * (XM_PI / 180.0f));

	if (m_inputManager->IsKeyPressedOnce(DIK_SPACE)) m_player->ChangeState();

	if (m_inputManager->getMouseData().leftClick && !m_inputManager->getPrevMouseData().leftClick)
	{
		m_player->Attack();
	}

	if (m_inputManager->IsKeyPressedOnce(DIK_TAB))
	{
		/*
		char outputString[75];
		sprintf_s(outputString, "Position: (%f,%f,%f)\n\n", m_position.x, m_position.y, m_position.z);
		OutputDebugString(outputString);
		*/

		char outputString[50];
		sprintf_s(outputString, "Health: %d/%d\n", m_player->getHealth(), m_player->getMaxHealth());
		OutputDebugString(outputString);
		sprintf_s(outputString, "Ammo: %d/%d\n\n", m_player->getAmmo(), m_player->getMaxAmmo());
		OutputDebugString(outputString);
	}
}