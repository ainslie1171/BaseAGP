#include "gameObject.h"

GameObject::GameObject(const GAMEOBJECT_DESC& desc)
{
	m_rootGameObject = desc.rootGameObject;
	m_pInputManager = desc.inputManager;
	m_pModelManager = desc.modelManager;
	m_pLightManager = desc.lightManager;
	m_pShaderManager = desc.shaderManager;
	m_pTextureManager = desc.textureManager;
	m_pParticleManager = desc.particleManager;
	m_pModel = desc.model;
	m_shaderID = desc.targetShader;
	m_textureID = desc.targetTexture;
	m_pMaterial = desc.material;
	m_canBeShot = desc.canBeShot;
	m_Scale = 1.0f;
	m_markedForRemoval = false;
	m_Position = ZeroVector3;

	if (m_pModel)
	{
		//Calculate Model Center Point
		float maxX = m_pModel->vertices[0].Pos.x, minX = m_pModel->vertices[0].Pos.x,
			maxY = m_pModel->vertices[0].Pos.y, minY = m_pModel->vertices[0].Pos.y,
			maxZ = m_pModel->vertices[0].Pos.z, minZ = m_pModel->vertices[0].Pos.z;
		for (UINT i = 1; i < m_pModel->numverts; i++)
		{
			if (maxX < m_pModel->vertices[i].Pos.x) maxX = m_pModel->vertices[i].Pos.x;
			if (minX > m_pModel->vertices[i].Pos.x) minX = m_pModel->vertices[i].Pos.x;
			if (maxY < m_pModel->vertices[i].Pos.y) maxY = m_pModel->vertices[i].Pos.y;
			if (minY > m_pModel->vertices[i].Pos.y) minY = m_pModel->vertices[i].Pos.y;
			if (maxZ < m_pModel->vertices[i].Pos.z) maxZ = m_pModel->vertices[i].Pos.z;
			if (minZ > m_pModel->vertices[i].Pos.z) minZ = m_pModel->vertices[i].Pos.z;
		}
		m_boundingShpere.x = minX + ((maxX - minX) / 2);
		m_boundingShpere.y = minY + ((maxY - minY) / 2);
		m_boundingShpere.z = minZ + ((maxZ - minZ) / 2);
		m_boundingShpere.w = 0;

		float	dx, dy, dz;
		for (unsigned int i = 0; i < m_pModel->numverts; i++)
		{
			dx = pow(m_pModel->vertices[i].Pos.x - m_boundingShpere.x, 2);
			dy = pow(m_pModel->vertices[i].Pos.y - m_boundingShpere.y, 2);
			dz = pow(m_pModel->vertices[i].Pos.z - m_boundingShpere.z, 2);
			if ((dx + dy + dz) > m_boundingShpere.w)
				m_boundingShpere.w = (dx + dy + dz);
		}
		m_boundingShpere.w = sqrtf(m_boundingShpere.w);
	}
}

GameObject::~GameObject()
{
	if (m_pParent) m_pParent->removeChild(this);

	while (!m_pChildren.empty())
	{
		delete m_pChildren.back();
	}
}

void GameObject::Update(float deltaTime)
{
	m_deltaTime = deltaTime;
	for (GameObject* c : m_pChildren)
	{
		c->Update(deltaTime);
		c->Update();
	}
}

void GameObject::Render(RENDER_DESC& desc)
{
	XMMATRIX WorldSpace = XMMatrixScaling(m_Scale, m_Scale, m_Scale);
	WorldSpace *= XMMatrixRotationRollPitchYawFromVector(m_Rotation.getXMVector());
	WorldSpace *= XMMatrixTranslationFromVector(m_Position.getXMVector());
	WorldSpace *= (*desc.world);

	RENDER_DESC ObjectDesc = desc;
	ObjectDesc.world = &WorldSpace;

	if (m_pModel)
	{
		if (m_textureID != ObjectDesc.targetTexture) updateTexture(ObjectDesc);
		if (m_shaderID != ObjectDesc.targetShader) updateShader(ObjectDesc);

		//create constatnt buffer
		REFLECT_CONSTANT_BUFFER model_cb_values;
		ZeroMemory(&model_cb_values, sizeof(REFLECT_CONSTANT_BUFFER));

		//add world view projection
		model_cb_values.WorldViewProjection = (*ObjectDesc.world)*(*ObjectDesc.view)*(*ObjectDesc.projection);
		model_cb_values.WorldMatrix = *ObjectDesc.world;
		model_cb_values.cameraPosition = *ObjectDesc.camera;
		model_cb_values.WorldViewMatrix = (*ObjectDesc.world) * (*ObjectDesc.view);

		//render lights
		model_cb_values.ambient_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

		m_pLightManager->renderLights(*desc.world, &model_cb_values);

		//material stuff
		if (m_pMaterial)
		{
			m_pMaterial->useTexture = (ObjectDesc.targetTexture > 0 ? 1 : 0);
			model_cb_values.material = *m_pMaterial;
		}

		m_pShaderManager->getShader(m_shaderID)->updateResources(&model_cb_values, ObjectDesc.skyboxTexture);

		//draw
		m_pModel->Draw();
	}

	for (GameObject* c : m_pChildren)
	{
		c->Render(ObjectDesc);
	}
}

void GameObject::updateShader(RENDER_DESC& desc)
{
	m_pShaderManager->updateShader(m_shaderID);
	desc.targetShader = m_shaderID;
}

void GameObject::updateTexture(RENDER_DESC& desc)
{
	if (!m_pTextureManager->setTexture(m_textureID))
	{
		if (m_textureID != 0)
		{
			OutputDebugString("Failed to update texture");
		}
	}
	desc.targetTexture = m_textureID;
}

void GameObject::Remove(void)
{
	m_markedForRemoval = true;
}

void GameObject::addChild(GameObject* child)
{
	m_pChildren.push_back(child);
	child->setParent(this);
}

void GameObject::cleanUp(void)
{
	if (m_markedForRemoval)
	{
		this->~GameObject();
	}
	else
	{
		for (UINT i = 0; i < m_pChildren.size(); i++)
		{
			m_pChildren.at(i)->cleanUp();
		}
	}
}

void GameObject::removeChild(GameObject* child)
{
	vector<GameObject*>::iterator it;
	it = find(m_pChildren.begin(), m_pChildren.end(), child);
	m_pChildren.erase(it);
}

void GameObject::updateCollisionTree(XMMATRIX* parentWorld, XMVECTOR parentScale)
{
	m_worldSpace = XMMatrixScaling(m_Scale, m_Scale, m_Scale);
	m_worldSpace *= XMMatrixRotationRollPitchYawFromVector(m_Rotation.getXMVector());
	m_worldSpace *= XMMatrixTranslationFromVector(m_Position.getXMVector());
	m_worldSpace *= *parentWorld;

	m_worldScale = parentScale * m_Scale;

	XMVECTOR s, r;
	XMMatrixDecompose(&s, &r, &m_worldCenter, m_worldSpace);

	if (m_pModel)  m_worldCenter += ((Vector3)m_boundingShpere).getXMVector();


	
	for (GameObject* c : m_pChildren)
	{
		c->updateCollisionTree(&m_worldSpace, m_worldScale);
	}
	
	
}

bool GameObject::checkCollision(GameObject* compareTree, GameObject* objectTreeRoot)
{
	if (compareTree == objectTreeRoot)
		return false;

	if (m_pModel && compareTree->m_pModel)
	{
		XMVECTOR compareTreeCenter = compareTree->getWorldCenter();

		float distance = distanceBetweenVectorsSqr(m_worldCenter, compareTreeCenter),
			compTreeRadius = compareTree->GetBoundingSphereRadius() * compareTree->m_worldScale.x,
			radius = (m_boundingShpere.w * m_worldScale.x) + compTreeRadius;

		radius *= radius;

		if (distance < radius)
		{
			Model *model = compareTree->getModel();

			Vector4 vertexA, vertexB, vertexC;
			for (unsigned int i = 0; i < model->numverts; i += 3)
			{
				vertexA = model->vertices[i].Pos;
				vertexB = model->vertices[i + 1].Pos;
				vertexC = model->vertices[i + 2].Pos;


				vertexA = XMVector3Transform(vertexA.getXMVector(), compareTree->m_worldSpace);
				vertexB = XMVector3Transform(vertexB.getXMVector(), compareTree->m_worldSpace);
				vertexC = XMVector3Transform(vertexC.getXMVector(), compareTree->m_worldSpace);

				if (checkCollisionRaySphere(vertexA, vertexB, false))
					return true;

				if (checkCollisionRaySphere(vertexB, vertexC, false))
					return true;

				if (checkCollisionRaySphere(vertexC, vertexA, false))
					return true;

			}


			for (unsigned int i = 0; i < model->numverts; i += 3)
			{
				vertexA = m_pModel->vertices[i].Pos;
				vertexB = m_pModel->vertices[i + 1].Pos;
				vertexC = m_pModel->vertices[i + 2].Pos;

				vertexA = XMVector3Transform(vertexA.getXMVector(), m_worldSpace);
				vertexB = XMVector3Transform(vertexB.getXMVector(), m_worldSpace);
				vertexC = XMVector3Transform(vertexC.getXMVector(), m_worldSpace);

				if (compareTree->checkCollisionRaySphere(vertexA, vertexB, false))
					return true;

				if (compareTree->checkCollisionRaySphere(vertexB, vertexC, false))
					return true;

				if (compareTree->checkCollisionRaySphere(vertexC, vertexA, false))
					return true;

			}

		}

	}

	for (GameObject* c : compareTree->m_pChildren)
	{
		if (checkCollision(c, objectTreeRoot))
			return true;
	}

	for (GameObject* c : m_pChildren)
	{
		if (c->checkCollision(compareTree, objectTreeRoot))
			return true;
	}

	return false;
}

bool GameObject::checkCollisionRaySphere(const Vector4& rayPointA, const Vector4& rayPointB, bool checkChildren)
{
	if (m_pModel)
	{
		float distanceSqr = distanceBetweenVectorsSqr(rayPointB, m_worldCenter);

		float rayLengthRadiusSqr = m_boundingShpere.w * m_worldScale.x;

		rayLengthRadiusSqr *= rayLengthRadiusSqr;

		if (distanceSqr <= rayLengthRadiusSqr)
		{
			if (checkCollisionRay(rayPointA, rayPointB))
				return true;
		}
	}

	if (checkChildren)
	{
		for (GameObject* c : m_pChildren)
		{
			if (c->checkCollisionRaySphere(rayPointA, rayPointB))
				return true;
		}
	}
	return false;
}

bool GameObject::checkCollisionRaySphereWRef(const Vector4& rayPointA, const Vector4& rayPointB, GameObject** collision, bool checkChildren)
{
	if (m_pModel)
	{
		float distanceSqr = distanceBetweenVectorsSqr(rayPointB, m_worldCenter);

		float rayLengthRadiusSqr = m_boundingShpere.w * m_worldScale.x;

		rayLengthRadiusSqr *= rayLengthRadiusSqr;

		if (distanceSqr <= rayLengthRadiusSqr)
		{
			if (checkCollisionRay(rayPointA, rayPointB))
			{
				*collision = this;
				return true;
			}
		}
	}

	if (checkChildren)
	{
		for (GameObject* c : m_pChildren)
		{
			if (c->checkCollisionRaySphereWRef(rayPointA, rayPointB, collision))
				return true;
		}
	}
	return false;
}

bool GameObject::checkCollisionRay(const Vector4& rayPointA, const Vector4& rayPointB)
{
	Vector4 ray = rayPointB - rayPointA;
	//XMVECTOR determinant;
	//XMMATRIX inverse = XMMatrixInverse(&determinant, m_worldSpace);
	//Vector4 pointA = XMVector3Transform(rayPointA.getXMVector(), inverse);

	Triangle triangle;
	Vector4 intersection;
	for (unsigned int i = 0; i < m_pModel->numverts; i += 3)
	{

		XMVECTOR vertexA = XMVectorSet(m_pModel->vertices[i].Pos.x,
			m_pModel->vertices[i].Pos.y,
			m_pModel->vertices[i].Pos.z, 0.0f);

		XMVECTOR vertexB = XMVectorSet(m_pModel->vertices[i + 1].Pos.x,
			m_pModel->vertices[i + 1].Pos.y,
			m_pModel->vertices[i + 1].Pos.z, 0.0f);

		XMVECTOR vertexC = XMVectorSet(m_pModel->vertices[i + 2].Pos.x,
			m_pModel->vertices[i + 2].Pos.y,
			m_pModel->vertices[i + 2].Pos.z, 0.0f);

		vertexA = XMVector3Transform(vertexA, m_worldSpace);
		vertexB = XMVector3Transform(vertexB, m_worldSpace);
		vertexC = XMVector3Transform(vertexC, m_worldSpace);

		Triangle triangle = { vertexA, vertexB, vertexC };

		if (planeRayIntersection(calculatePlane(triangle), rayPointA, ray, intersection))
		{
			if (trianglePointIntersection(triangle, intersection))
			{
				return true;
			}
		}
	}
	return false;
}

void GameObject::Move(Vector3 deltaPosition)
{
	m_Position += deltaPosition;
	XMMATRIX identity = XMMatrixIdentity();
	m_rootGameObject->updateCollisionTree(&identity, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	if (checkCollision(m_rootGameObject))
		m_Position -= deltaPosition;
}

void GameObject::rotateBy(Vector3 deltaRotation)
{
	UNREFERENCED_PARAMETER(deltaRotation);
}

void GameObject::drawBoundingSphere(RENDER_DESC& desc)
{
	
	XMMATRIX worldSpace = XMMatrixIdentity();
	RENDER_DESC modelDesc = desc;
	modelDesc.world = &worldSpace;
	
	float scale = 1.0f;
	if (m_pModel)scale = (m_boundingShpere.w / 2.93333411f);
	scale *= m_Scale;

	*modelDesc.world = XMMatrixScaling(m_Scale, m_Scale, m_Scale);
	*modelDesc.world *= XMMatrixRotationRollPitchYawFromVector(m_Rotation.getXMVector());
	if (m_pModel)
	{
		*modelDesc.world *= XMMatrixTranslationFromVector((m_Position + m_boundingShpere).getXMVector());
	}
	else
	{
		*modelDesc.world *= XMMatrixTranslationFromVector(m_Position.getXMVector());
	}
	*modelDesc.world *= (*desc.world);

	if (m_pModel)
	{
		REFLECT_CONSTANT_BUFFER model_cb_values;
		ZeroMemory(&model_cb_values, sizeof(REFLECT_CONSTANT_BUFFER));

		//add world view projection
		model_cb_values.WorldViewProjection = (*modelDesc.world)*(*modelDesc.view)*(*modelDesc.projection);
		model_cb_values.WorldMatrix = *modelDesc.world;
		//model_cb_values.cameraPosition = *modelDesc.camera;
		model_cb_values.WorldViewMatrix = (*modelDesc.world) * (*modelDesc.view);

		//render lights
		model_cb_values.ambient_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

		//material stuff
		if (m_pMaterial)
		{
			m_pMaterial->useTexture = (modelDesc.targetTexture > 0 ? 1 : 0);
			model_cb_values.material = *m_pMaterial;
		}

		if (m_pShaderManager->getShader(m_shaderID)) m_pShaderManager->getShader(m_shaderID)->updateResources(&model_cb_values, modelDesc.skyboxTexture);

		//draw
		m_pModelManager->getModel(1)->Draw();
	}

	for (GameObject* c : m_pChildren)
	{
		c->drawBoundingSphere(modelDesc);
	}
}

void GameObject::setPosition(Vector3 position) 
{ 
	m_Position = position;
}

void GameObject::Shot()
{
	if (m_canBeShot) Remove();
}