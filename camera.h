#ifndef _CAMERA_H_
#define _CAMERA_H_

//TODO Convert camera class so that is uses a matrix for movement
//TODO Convect camera class to not be a singleton

#include "maths.h"

class GameObject;

class Camera
{
private:
	Camera();

	float	m_speed;
	float 	m_rotatinSpeed;

	float m_pitch, m_yaw;

	XMMATRIX m_view;
	XMMATRIX m_projection;

	XMMATRIX m_inverseView;

	Vector3 m_position;
	Vector3	m_rotation;
	Vector3	m_lookAt;
	Vector3 m_up;

	GameObject* m_rootNode;
	

public:

	static Camera& getInstance()
	{
		static Camera instance;
		return instance;
	}

	Camera(Camera const&) = delete;
	void operator=(Camera const&) = delete;
	void move(const Vector4& deltaPosition);
	void setPosition(Vector3 position);
	Vector3 getPosition();

	void Rotate(float x, float y);
	void setLookAt(Vector3 lookAt);


	XMMATRIX getMatrixView();
	XMVECTOR getVectorView();

	float getSpeed(){ return m_speed; }
	float getRotationSpeed(){ return m_rotatinSpeed; }

	void setRootNode(GameObject* node){ m_rootNode = node; }
};

#endif