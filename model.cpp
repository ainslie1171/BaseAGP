#include "model.h"

Model::Model(const MODEL_DESC& desc)
{
	m_pD3DDevice = desc.device;
	m_pImmediateContext = desc.context;
	m_pLightManager = desc.lightManager;
	m_pShaderManager = desc.shaderManager;
	m_pTextureManager = desc.textureManager;
	LoadObjModel(desc.filePath);
	m_shaderID = desc.targetShader;
	m_textureID = desc.targetTexture;
	m_pMaterial = desc.material;
	m_scale = 1.0f;
}

//13
Model::~Model()
{
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	if (m_pObject) { delete m_pObject; m_pObject = nullptr; }
}

int Model::LoadObjModel(char* fileName)
{
	HRESULT hr = S_OK;

	m_pObject = new ObjFileModel(fileName, m_pD3DDevice, m_pImmediateContext);
	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	// setup the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //Can use UpdateSubresourse() to update
	constant_buffer_desc.ByteWidth = sizeof(REFLECT_CONSTANT_BUFFER); //MUST be a multiple of 16, calculated from the constant buffer struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //Use as a constant buffer

	//create the constant buffer
	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

	CalculateModelCenterPoint();
	CalculateBoundingSphereRadius();

	return S_OK;
}


void Model::Draw(RENDER_DESC& desc)
{
	if (m_textureID != desc.targetTexture) updateTexture(desc);
	if (m_shaderID != desc.targetShader) updateShader(desc);

	//create constatnt buffer
	REFLECT_CONSTANT_BUFFER model_cb_values;
	ZeroMemory(&model_cb_values, sizeof(REFLECT_CONSTANT_BUFFER));
	//add world view projection
	model_cb_values.WorldViewProjection = (*desc.world)*(*desc.view)*(*desc.projection);
	model_cb_values.WorldMatrix = *desc.world;
	model_cb_values.cameraPosition = *desc.camera;
	model_cb_values.WorldViewMatrix = (*desc.world) * (*desc.view);

	//render lights
	model_cb_values.ambient_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

	if (!m_unlit)
		m_pLightManager->renderLights(*desc.world, &model_cb_values);

	//material stuff
	if (m_pMaterial)
	{
		m_pMaterial->useTexture = (desc.targetTexture > 0 ? 1 : 0);
		model_cb_values.material = *m_pMaterial;
	}

	m_pShaderManager->getShader(m_shaderID)->updateResources(m_pImmediateContext, &model_cb_values, desc.skyboxTexture);
	

	//draw
	m_pObject->Draw();
}

void Model::CalculateModelCenterPoint()
{
	float maxX = m_pObject->vertices[0].Pos.x, minX = m_pObject->vertices[0].Pos.x, 
		maxY = m_pObject->vertices[0].Pos.y, minY = m_pObject->vertices[0].Pos.y,
		maxZ = m_pObject->vertices[0].Pos.z, minZ = m_pObject->vertices[0].Pos.z;
	for (unsigned int i = 1; i < m_pObject->numverts; i++)
	{
		if (maxX < m_pObject->vertices[i].Pos.x) maxX = m_pObject->vertices[i].Pos.x;
		if (minX > m_pObject->vertices[i].Pos.x) minX = m_pObject->vertices[i].Pos.x;
		if (maxY < m_pObject->vertices[i].Pos.y) maxY = m_pObject->vertices[i].Pos.y;
		if (minY > m_pObject->vertices[i].Pos.y) minY = m_pObject->vertices[i].Pos.y;
		if (maxZ < m_pObject->vertices[i].Pos.z) maxZ = m_pObject->vertices[i].Pos.z;
		if (minZ > m_pObject->vertices[i].Pos.z) minZ = m_pObject->vertices[i].Pos.z;
	}
	m_boundingShpere.x = minX + ((maxX - minX) / 2);
	m_boundingShpere.y = minY + ((maxY - minY) / 2);
	m_boundingShpere.z = minZ + ((maxZ - minZ) / 2);
}

void Model::CalculateBoundingSphereRadius()
{
	m_boundingShpere.w = 0;
	float	dx, dy, dz;
	for (unsigned int i = 0; i < m_pObject->numverts; i++)
	{
		dx = pow(m_pObject->vertices[i].Pos.x - m_boundingShpere.x, 2);
		dy = pow(m_pObject->vertices[i].Pos.y - m_boundingShpere.y, 2);
		dz = pow(m_pObject->vertices[i].Pos.z - m_boundingShpere.z, 2);
		if ((dx + dy + dz) > m_boundingShpere.w)
			m_boundingShpere.w = (dx + dy + dz);
	}
	m_boundingShpere.w = sqrtf(m_boundingShpere.w);
}

float Model::GetBoundingSphereRadius()
{
	return m_boundingShpere.w;
}

float Model::GetBoundingSphereRadiusSqr()
{
	return m_boundingShpere.w * m_boundingShpere.w;
}

void Model::updateShader(RENDER_DESC& desc)
{
	Shader s;
	if (!m_pShaderManager->getShader(m_shaderID, &s))
	{
		if (m_shaderID != 0) OutputDebugString("Failed to retrieve shader\n");
	}
	else
	{
		m_pImmediateContext->VSSetShader(s.VShader, 0, 0);
		m_pImmediateContext->PSSetShader(s.PShader, 0, 0);
		m_pImmediateContext->IASetInputLayout(s.InputLayout);
	}
	desc.targetShader = m_shaderID;
}

void Model::updateTexture(RENDER_DESC& desc)
{
	Texture t;
	if (!m_pTextureManager->getTexture(m_textureID, &t))
	{
		if (m_textureID != 0) OutputDebugString("Failed to retrieve texture");
	}
	else
	{
		m_pImmediateContext->PSSetSamplers(0, 1, &t.m_pSampler);
		m_pImmediateContext->PSSetShaderResources(0, 1, &t.m_pTexture);
	}
	desc.targetTexture = m_textureID;
}