#include "camera.h"
#include "sceneNode.h"

Camera::Camera()//float x, float y, float z, float cameraRotationX, float cameraRotationY)
{
	m_position = ZeroVector4;
	m_rotation = ZeroVector4;
	m_speed = 2.0f;
	m_rotatinSpeed = 180.0f;
}

void Camera::RotateX(float degreesOfRotation)
{
	m_rotation.x += degreesOfRotation;
	m_lookAt.x = sin(m_rotation.x * (XM_PI / 180.0f));
	m_lookAt.z = cos(m_rotation.x * (XM_PI / 180.0f));
}

void Camera::RotateY(float degreesOfRotation)
{
	if (m_rotation.y + degreesOfRotation < 90 && m_rotation.y + degreesOfRotation > -90)
	{
		m_rotation.y += degreesOfRotation;
	}

	m_lookAt.y = sin(m_rotation.y * (XM_PI / 180.0f));
	//m_dzy = cos(m_rotation.y * (XM_PI / 180.0));
}

void Camera::Forward(float distance)
{
	Vector4 moveTo = m_position;
	moveTo.x += m_lookAt.x * distance;
	moveTo.z += m_lookAt.z * distance;
	move(moveTo);
}

void Camera::Up(float distance)
{
	Vector4 moveTo = m_position;
	moveTo.y += distance;
	move(moveTo);
}

void Camera::Strafe(float distance)
{
	Vector4 moveTo = m_position;
	moveTo.x += sin((m_rotation.x + 90) * (XM_PI / 180.0f)) * distance;
	moveTo.z += cos((m_rotation.x + 90) * (XM_PI / 180.0f)) * distance;
	move(moveTo);
}

void Camera::move(const Vector4& destination)
{
	XMMATRIX identity = XMMatrixIdentity();
	m_rootNode->updateCollisionTree(&identity, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	if (!m_rootNode->checkCollisionRaySphere(m_position, destination))
		m_position = destination;
}

XMMATRIX Camera::getMatrixView()
{
	//m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	//m_lookAt = XMVectorSet(m_x + m_dx, m_y + m_dy , m_z + m_dzx, 0.0);
	//m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	return XMMatrixLookAtLH(m_position.getXMVector(), (m_position + m_lookAt).getXMVector(), XMVectorSet(0.0, 1.0, 0.0, 0.0));
}
XMVECTOR Camera::getVectorView()
{
	return m_lookAt.getXMVector();
}

void Camera::setPosition(Vector4 position)
{
	m_position.x = position.x;
	m_position.y = position.y;
	m_position.z = position.z;
}