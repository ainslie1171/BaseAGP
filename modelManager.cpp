#include "modelManager.h"

ModelManager::ModelManager(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_pD3DDevice = device;
	m_pImmediateContext = context;
}

ModelManager::~ModelManager()
{
	while (!m_pModels.empty())
	{
		delete m_pModels.back();
		m_pModels.pop_back();
	}
}

void ModelManager::LoadModel(char* filePath)
{
	UINT ID = 1;
	if (!m_pModels.empty()) ID = m_pModels.back()->getID() + 1;
	Model* m = new Model(filePath, m_pD3DDevice, m_pImmediateContext, ID);
	m_pModels.push_back(m);
}

Model* ModelManager::getModel(UINT ID)
{
	for (Model* m : m_pModels)
	{
		if (m->getID() == ID) return m;
	}
	return nullptr;
}