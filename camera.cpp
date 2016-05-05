#include "camera.h"
#include "gameObject.h"

Camera::Camera()
{
	m_view = XMMatrixIdentity();
	m_projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 640.0f / 480.0f, 0.01f, 100.0f);
	m_inverseView = XMMatrixIdentity();
	m_position = ZeroVector4;
	m_rotation = ZeroVector4;
	m_speed = 2.0f;
	m_rotatinSpeed = 180.0f;
	m_lookAt = { 0.0f, 0.0f, 1.0f };
	m_up = { 0.0f, 1.0f, 0.0f };
}

//rotate
void Camera::Rotate(float x, float y)
{
	m_pitch -= y;
	m_yaw += x;

	float limit = XM_PI / 2.0f - 0.01f;
	m_pitch = __max(-limit, m_pitch);
	m_pitch = __min(+limit, m_pitch);
	if (m_yaw >  XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}
}

//move

void Camera::move(const Vector4& deltaPosition)
{
	m_position += deltaPosition;
}

XMMATRIX Camera::getMatrixView()
{
	return XMMatrixLookAtLH(m_position.getXMVector(), (m_position + m_lookAt).getXMVector(), XMVectorSet(0.0, 1.0, 0.0, 0.0));
}

XMVECTOR Camera::getVectorView()
{
	return m_lookAt.getXMVector();
}

Vector3 Camera::getPosition()
{
	return m_position;
}

void Camera::setPosition(Vector3 position)
{
	m_position = position;
}

void Camera::setLookAt(Vector3 lookAt)
{
	m_lookAt = lookAt;
}