#include "camera.h"

Camera::Camera()
{
	m_position = ZeroVector3;
	m_speed = 10.0f;
	m_rotatinSpeed = 25.0f;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_transform = XMMatrixIdentity();
}

void Camera::RotateY(float degreesOfRotation)
{
	m_pitch += degreesOfRotation;

}

void Camera::RotateX(float degreesOfRotation)
{
	if (m_transform.m[1][1] >= 0)
	{
		m_yaw += degreesOfRotation;
	}
	else
	{
		m_yaw -= degreesOfRotation;
	}
}

void Camera::Forward(float distance)
{
	m_position += (Vector3)m_transform.m[2] * distance;
}

void Camera::Up(float distance)
{
	m_position += (Vector3)m_transform.m[1] * distance;
}

void Camera::Strafe(float distance)
{
	m_position += (Vector3)m_transform.m[0] * distance;
}


XMMATRIX Camera::getMatrixView()
{
	m_transform = XMMatrixIdentity();
	m_transform *= XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0.0f);
	m_transform *= XMMatrixTranslationFromVector(m_position.getXMVector());
	XMVECTOR d;
	return XMMatrixInverse(&d, m_transform);
}

void Camera::setPosition(Vector4 position)
{
	m_position = position;
}