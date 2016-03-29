#include "sceneNode.h"

SceneNode::SceneNode(SceneNode* rootNode, Model* model)
{
	m_pModel = model;
	m_rootNode = rootNode;
	m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	//const float m_bounindgModelRadius = 2.93333411f;
}

SceneNode::~SceneNode()
{
	m_children.clear();
}

void SceneNode::addNode(SceneNode* node)
{
	m_children.push_back(node);
}

bool SceneNode::removeNode(SceneNode* node)
{
	//ask Pete about the vector for loop thing
	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		if (node == m_children[i])
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}
		if (m_children[i]->removeNode(node) == true) 
			return true;
	}
	return false;
}

void SceneNode::draw(const XMMATRIX& parent, const XMMATRIX& view, const XMMATRIX& projection)
{
	XMMATRIX worldSpace = XMMatrixIdentity();

	worldSpace = XMMatrixScalingFromVector(m_scale);
	worldSpace *= XMMatrixRotationRollPitchYawFromVector(m_rotation);
	worldSpace *= XMMatrixTranslationFromVector(m_position);
	worldSpace *= parent;

	if (m_pModel) m_pModel->Draw(worldSpace, view, projection);

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->draw(worldSpace, view, projection);
	}
	
}

void SceneNode::drawBoundingSphere(const XMMATRIX& parent, const XMMATRIX& view, const XMMATRIX& projection)
{
	XMMATRIX worldSpace = XMMatrixIdentity();

	float scale = 1.0f;
	if(m_pModel) scale = m_pModel->GetBoundingSphereRadius();
	if(m_pBoundingModel) scale /= m_pBoundingModel->GetBoundingSphereRadius();
	scale *= m_scale.x;
	worldSpace = XMMatrixScaling(scale, scale, scale);
	worldSpace *= XMMatrixRotationRollPitchYawFromVector(m_rotation);
	worldSpace *= XMMatrixTranslationFromVector(m_position);
	worldSpace *= parent;

	if (m_pBoundingModel) m_pBoundingModel->Draw(worldSpace, view, projection);

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->drawBoundingSphere(worldSpace, view, projection);
	}
}

void SceneNode::addBoundingSphereModel(Model* model)
{
	m_pBoundingModel = model;
}

void SceneNode::updateCollisionTree(XMMATRIX* parentWorld, XMVECTOR parentScale)
{
	m_worldSpace = XMMatrixIdentity();

	m_worldSpace = XMMatrixScalingFromVector(m_scale);
	m_worldSpace *= XMMatrixRotationRollPitchYawFromVector(m_rotation);
	m_worldSpace *= XMMatrixTranslationFromVector(m_position);
	m_worldSpace *= *parentWorld;

	m_worldScale = parentScale * m_scale;

	XMVECTOR boundingSpherePosition = XMVectorZero();

	if (m_pModel)
		boundingSpherePosition = m_pModel->GetBoundingSphereWorldSpacePosition();

	m_worldCenter = XMVector3Transform(boundingSpherePosition, m_worldSpace);

	for (SceneNode* node : m_children)
	{
		node->updateCollisionTree(&m_worldSpace, m_worldScale);
	}
}

bool SceneNode::checkCollision(SceneNode* compareTree, SceneNode* objectTreeRoot)
{
	if (compareTree == objectTreeRoot)
		return false;

	if (m_pModel && compareTree->m_pModel)
	{
		XMVECTOR compareTreeCenter = compareTree->getWorldCenterPosition();

		float distance = distanceBetweenVectorsSqr(m_worldCenter, compareTreeCenter),
			compTreeRadius = compareTree->m_pModel->GetBoundingSphereRadius() * compareTree->m_worldScale.x,
			radius = (m_pModel->GetBoundingSphereRadius() * m_worldScale.x) + compTreeRadius;

		radius *= radius;
		if (distance < radius)
		{
			ObjFileModel *model = compareTree->m_pModel->getObject();
			for (unsigned int i = 0; i < model->numverts; i += 3)
			{
				XMVECTOR vertexA = XMVectorSet(model->vertices[i].Pos.x,
					model->vertices[i].Pos.y,
					model->vertices[i].Pos.z, 0.0f);

				XMVECTOR vertexB = XMVectorSet(model->vertices[i + 1].Pos.x,
					model->vertices[i + 1].Pos.y,
					model->vertices[i + 1].Pos.z, 0.0f);

				XMVECTOR vertexC = XMVectorSet(model->vertices[i + 2].Pos.x,
					model->vertices[i + 2].Pos.y,
					model->vertices[i + 2].Pos.z, 0.0f);

				vertexA = XMVector3Transform(vertexA, compareTree->m_worldSpace);
				vertexB = XMVector3Transform(vertexB, compareTree->m_worldSpace);
				vertexC = XMVector3Transform(vertexC, compareTree->m_worldSpace);
				
				if (checkCollisionRaySphere(vertexA, vertexB, false))
					return true;

				if (checkCollisionRaySphere(vertexB, vertexC, false))
					return true;

				if (checkCollisionRaySphere(vertexC, vertexA, false))
					return true;
					
			}

			model = m_pModel->getObject();
			for (unsigned int i = 0; i < model->numverts; i += 3)
			{
				XMVECTOR vertexA = XMVectorSet(model->vertices[i].Pos.x,
					model->vertices[i].Pos.y,
					model->vertices[i].Pos.z, 0.0f);

				XMVECTOR vertexB = XMVectorSet(model->vertices[i + 1].Pos.x,
					model->vertices[i + 1].Pos.y,
					model->vertices[i + 1].Pos.z, 0.0f);

				XMVECTOR vertexC = XMVectorSet(model->vertices[i + 2].Pos.x,
					model->vertices[i + 2].Pos.y,
					model->vertices[i + 2].Pos.z, 0.0f);

				vertexA = XMVector3Transform(vertexA, m_worldSpace);
				vertexB = XMVector3Transform(vertexB, m_worldSpace);
				vertexC = XMVector3Transform(vertexC, m_worldSpace);
				
				if (compareTree->checkCollisionRaySphere(vertexA, vertexB, false))
					return true;

				if (compareTree->checkCollisionRaySphere(vertexB, vertexC, false))
					return true;

				if (compareTree->checkCollisionRaySphere(vertexC, vertexA, false))
					return true;
					
			}

		}
			
	}

	for (SceneNode* node : compareTree->m_children)
	{
		if (checkCollision(node, objectTreeRoot))
			return true;
	}

	for (SceneNode* node : m_children)
	{
		if (node->checkCollision(compareTree, objectTreeRoot))
			return true;
	}

	return false;
}

void SceneNode::forward(float distance)
{
	move(distance, 1);
	XMMATRIX identity = XMMatrixIdentity();
	m_rootNode->updateCollisionTree(&identity, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	if (checkCollision(m_rootNode))
		move(-distance, 1);
}

void SceneNode::backward(float distance)
{
	move(-distance, 1);
	XMMATRIX identity = XMMatrixIdentity();
	m_rootNode->updateCollisionTree(&identity, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	if (checkCollision(m_rootNode))
		move(distance, 1);
}

void SceneNode::left(float distance)
{
	move(-distance, 0);
	XMMATRIX identity = XMMatrixIdentity();
	m_rootNode->updateCollisionTree(&identity, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	if (checkCollision(m_rootNode))
		move(distance, 0);
}

void SceneNode::right(float distance)
{
	move(distance, 0);
	XMMATRIX identity = XMMatrixIdentity();
	m_rootNode->updateCollisionTree(&identity, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	if (checkCollision(m_rootNode))
		move(-distance, 0);
}

void SceneNode::move(float distance, bool direction)
{
	if (direction)
	{
		m_position = XMVectorSet(m_position.x, m_position.y, (m_position.z + distance), m_position.w);
	}
	else
	{
		m_position = XMVectorSet((m_position.x + distance), m_position.y, m_position.z, m_position.w);
	}
}

bool SceneNode::checkCollisionRaySphere(const Vector4& rayPointA, const Vector4& rayPointB, bool checkChildren)
{
	if (m_pModel)
	{
		//float distanceSqr = distanceBetweenVectorsSqr(rayPointB, m_worldCenter);
		//float rayLengthRadiusSqr = distanceBetweenVectors(rayPointA, rayPointB) + (m_pModel->GetBoundingSphereRadius() * m_worldScale.x);// *m_worldScale.x));
		//rayLengthRadiusSqr *= rayLengthRadiusSqr;

		float distanceSqr = distanceBetweenVectorsSqr(rayPointB, m_worldCenter);
		float rayLengthRadiusSqr = m_pModel->GetBoundingSphereRadius() * m_worldScale.x;
		rayLengthRadiusSqr *= rayLengthRadiusSqr;
		if (distanceSqr <= rayLengthRadiusSqr)
		{
			if (checkCollisionRay(rayPointA, rayPointB))
				return true;
		}
	}

	if (checkChildren)
	{
		for (SceneNode* node : m_children)
		{
			if (node->checkCollisionRaySphere(rayPointA, rayPointB))
				return true;
		}
	}
	return false;
}

bool SceneNode::checkCollisionRay(const Vector4& rayPointA, const Vector4& rayPointB)
{
	
	ObjFileModel *model = m_pModel->getObject();
	for (unsigned int i = 0; i < model->numverts; i+=3)
	{
		XMVECTOR vertexA = XMVectorSet(model->vertices[i].Pos.x,
			model->vertices[i].Pos.y,
			model->vertices[i].Pos.z, 0.0f);

		XMVECTOR vertexB = XMVectorSet(model->vertices[i+1].Pos.x,
			model->vertices[i+1].Pos.y,
			model->vertices[i+1].Pos.z, 0.0f);

		XMVECTOR vertexC = XMVectorSet(model->vertices[i+2].Pos.x,
			model->vertices[i+2].Pos.y,
			model->vertices[i+2].Pos.z, 0.0f);

		vertexA = XMVector3Transform(vertexA, m_worldSpace);
		vertexB = XMVector3Transform(vertexB, m_worldSpace);
		vertexC = XMVector3Transform(vertexC, m_worldSpace);

		Triangle triangle = { vertexA, vertexB, vertexC };
		Plane plane = calculatePlane(triangle);
		Vector4 intersection;
		if (planeIntersection(plane, rayPointA, rayPointB, intersection))
		{
			if (trianglePointIntersection(triangle, intersection))
			{
				return true;
			}
		}
	}
	return false;
}