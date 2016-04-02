#ifndef _SCENE_NODE_H_
#define _SCENE_NODE_H_

#include "model.h"
#include "maths.h"

class LightManager;

class SceneNode
{
private:
	Model* m_pModel;
	Model* m_pBoundingModel;
	SceneNode* m_rootNode;
	vector<SceneNode*> m_children;
	XMVECTOR m_position, m_rotation, m_scale, m_worldCenter, m_worldScale;
	XMMATRIX m_worldSpace;


public:
	SceneNode(SceneNode* rootNode=nullptr, Model* model = nullptr);
	~SceneNode();
	
	XMVECTOR getPosition(){ return m_position; };
	void setPosition(float x, float y, float z, float w = 0.0f){ m_position = XMVectorSet(x, y, z, w); };
	void setPosition(XMVECTOR position){ m_position = position; };

	XMVECTOR getRotation(){ return m_rotation; };
	void setRotation(float x, float y, float z, float w = 0.0f){ m_rotation = XMVectorSet(x, y, z, w); };
	void setRotation(XMVECTOR rotation){ m_rotation = rotation; };

	XMVECTOR getScale(){ return m_scale; };
	void setScale(float x, float y, float z, float w = 0.0f){ m_scale = XMVectorSet(x, y, z, w); };
	void setScale(XMVECTOR scale){ m_scale = scale; };

	void addNode(SceneNode* node);
	bool removeNode(SceneNode* node);
	void draw(RENDER_DESC& desc);
	void updateCollisionTree(XMMATRIX* parentWorld, XMVECTOR parentScale);
	XMVECTOR getWorldCenterPosition(){ return m_worldCenter; }
	XMVECTOR getWorldScale(){ return m_worldScale; };

	bool checkCollision(SceneNode* compareTree) { return checkCollision(compareTree, this); };
	bool checkCollision(SceneNode* compareTree, SceneNode* objectTreeRoot);
	bool checkCollisionRaySphere(const Vector4& rayPointA, const Vector4& rayPointB, bool checkChildren = true);
	bool checkCollisionRay(const Vector4& rayPointA, const Vector4& rayPointB);

	void forward(float distance);
	void backward(float distance);
	void left(float distance);
	void right(float distance);
	void move(float distance, bool direction);

	void drawBoundingSphere(RENDER_DESC& desc);
	void addBoundingSphereModel(Model* model);


};

#endif