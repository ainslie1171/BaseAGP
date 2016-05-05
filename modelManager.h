#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "model.h"

class ModelManager
{
private:
	std::vector<Model*> m_pModels;
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
public:
	ModelManager(ID3D11Device* device, ID3D11DeviceContext* context);
	~ModelManager();
	void LoadModel(char* filePath);
	Model* getModel(UINT ID);
};

#endif//_MODELMANAGER_H_

/*
Change Log:

*/