#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "maths.h"
#include "inputManager.h"
#include "lightManager.h"
#include "shaderManager.h"
#include "textureManager.h"
#include "particleManager.h"
#include "modelManager.h"

class GameObject;

struct RENDER_DESC
{
	XMVECTOR* camera;
	XMVECTOR* viewVector;
	XMMATRIX* world;
	XMMATRIX* view;
	XMMATRIX* projection;
	ID3D11ShaderResourceView* skyboxTexture;
	UINT targetShader;
	UINT targetTexture;
	float deltaTime;
};

struct GAMEOBJECT_DESC
{
	InputManager* inputManager;
	GameObject* rootGameObject;
	ModelManager* modelManager;
	LightManager* lightManager;
	ShaderManager* shaderManager;
	TextureManager* textureManager;
	ParticleManager* particleManager;
	Model* model;
	UINT targetShader;
	UINT targetTexture;
	Material* material;
	bool canBeShot;
};

class GameObject
{
protected:
	GameObject* m_rootGameObject;
	Model* m_pModel;
	UINT m_shaderID;
	UINT m_textureID;
	Material* m_pMaterial;
	LightManager* m_pLightManager;
	ShaderManager* m_pShaderManager;
	TextureManager* m_pTextureManager;
	ModelManager* m_pModelManager;
	ParticleManager* m_pParticleManager;
	Vector3 m_Rotation;
	float m_Speed;
	float m_Scale;
	GameObject* m_pParent;
	vector<GameObject*> m_pChildren;
	XMMATRIX m_worldSpace;
	XMVECTOR m_worldCenter;
	XMVECTOR m_worldScale;
	Vector4 m_boundingShpere;
	float m_deltaTime;
	bool m_canBeShot;
	
	void updateShader(RENDER_DESC& desc);
	void updateTexture(RENDER_DESC& desc);

	InputManager* m_pInputManager;
	Vector3 m_Position;

	bool m_markedForRemoval;
	void Move(Vector3 deltaPosition);
	void rotateBy(Vector3 deltaRotation);
	
public:
	GameObject(const GAMEOBJECT_DESC& desc);
	~GameObject(void);
	void virtual Update(float deltaTime);
	void virtual Update(void){};
	void Render(RENDER_DESC& desc);
	void drawBoundingSphere(RENDER_DESC& desc);
	void Remove(void);
	void addChild(GameObject* child);
	void removeChild(GameObject* child);
	void setParent(GameObject* parent) { m_pParent = parent; }
	void virtual setPosition(Vector3 position);
	void setRotation(Vector3 rotation) { m_Rotation = rotation; }
	void setScale(float scale) { m_Scale = scale; }
	void cleanUp(void);
	Model* getModel(){ return m_pModel; }
	void updateCollisionTree(XMMATRIX* parentWorld, XMVECTOR parentScale);
	bool checkCollision(GameObject* compareTree) { return checkCollision(compareTree, this); };
	bool checkCollision(GameObject* compareTree, GameObject* objectTreeRoot);
	bool checkCollisionRay(const Vector4& rayPointA, const Vector4& rayPointB);
	bool checkCollisionRaySphere(const Vector4& rayPointA, const Vector4& rayPointB, bool checkChildren = true);
	bool checkCollisionRaySphereWRef(const Vector4& rayPointA, const Vector4& rayPointB, GameObject** collision, bool checkChildren = true);
	float GetBoundingSphereRadius() { return m_boundingShpere.w; }
	XMVECTOR getWorldCenter() { return m_worldCenter; }
	GameObject* getRoot(){ return m_rootGameObject; }
	void Shot(void);
	void setCanBeShot(bool b){ m_canBeShot = b; }
};

#endif//_GAMEOBJECT_H_

/*
Change Log:

*/