#ifndef _CAMERA_H_
#define _CAMERA_H_

//TODO Convert camera class so that is uses a matrix for movement
//TODO Convect camera class to not be a singleton

#include "maths.h"

class SceneNode;

class Camera
{
private:
	Camera();

	float	m_speed;
	float 	m_rotatinSpeed;

	Vector4 m_position;
	Vector4	m_rotation;
	Vector4	m_lookAt;

	SceneNode* m_rootNode;
	void move(const Vector4& destination);

public:

	static Camera& getInstance()
	{
		static Camera instance;
		return instance;
	}

	Camera(Camera const&) = delete;
	void operator=(Camera const&) = delete;

	void setPosition(Vector4 position);
	Vector4 getPosition(){ return m_position; }

	void RotateX(float degreesOfRotation);
	void RotateY(float degreesOfRotation);

	void Forward(float distance);
	void Up(float distance);
	void Strafe(float distance);

	XMMATRIX getMatrixView();
	XMVECTOR getVectorView();

	float getSpeed(){ return m_speed; }
	float getRotationSpeed(){ return m_rotatinSpeed; }

	void setRootNode(SceneNode* node){ m_rootNode = node; }
};

#endif