#ifndef _CAMERA_H_
#define _CAMERA_H_



//#include <math.h>
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

	XMVECTOR m_XMposition;
	XMVECTOR m_XMlookAt;
	XMVECTOR m_XMup;

	SceneNode* m_rootNode;

public:

	static Camera& getInstance()
	{
		static Camera instance;
		return instance;
	}

	Camera(Camera const&) = delete;
	void operator=(Camera const&) = delete;
	void setPosition(Vector4 position);
	void RotateX(float degreesOfRotation);
	void RotateY(float degreesOfRotation);
	void Forward(float distance);
	void Up(float distance);
	void Strafe(float distance);
	XMMATRIX getMatrixView();
	XMVECTOR getVectorView();

	float getSpeed(){ return m_speed; };
	float getRotationSpeed(){ return m_rotatinSpeed; };
	Vector4 getPosition(){ return m_position; };
	void setRootNode(SceneNode* node){ m_rootNode = node; };
	void move(const Vector4& destination);

};

#endif