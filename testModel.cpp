#include "testModel.h"

const XMVECTOR TestModel::xAxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR TestModel::yAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR TestModel::zAxis = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

TestModel::TestModel(const testModelDesc& desc)
{
	m_pD3DDevice = desc.device;
	m_pImmediateContext = desc.context;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_zAngle = 0.0f;
	m_scale = 1.0f;
	m_pLightManager = desc.lightManager;
	m_pShaderManager = desc.shaderManager;

	m_transform = XMMatrixIdentity();
	m_transform *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
}

//13
TestModel::~TestModel()
{
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	if (m_pObject) { delete m_pObject; m_pObject = nullptr; }
}

int TestModel::LoadObjModel(char* fileName)
{
	HRESULT hr = S_OK;

	m_pObject = new ObjFileModel(fileName, m_pD3DDevice, m_pImmediateContext);
	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	// setup the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //Can use UpdateSubresourse() to update
	constant_buffer_desc.ByteWidth = sizeof(TEST_CONSTANT_BUFFER); //MUST be a multiple of 16, calculated from the constant buffer struct
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

int TestModel::AddTexture(char* fileName)
{
	HRESULT hr = S_OK;
	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, fileName, NULL, NULL, &m_pTexture, NULL);
	//Craete the Sampler State
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler);

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}


void TestModel::Draw(RenderInfo* data)
{
	//create constatnt buffer
	REFLECT_CONSTANT_BUFFER model_cb_values;
	ZeroMemory(&model_cb_values, sizeof(REFLECT_CONSTANT_BUFFER));
	*data->world *= XMMatrixTranslation(m_x, m_y, m_z);
	//add world view projection
	model_cb_values.WorldViewProjection = (*data->world)*(*data->view)*(*data->projection);
	model_cb_values.WorldMatrix = *data->world;
	model_cb_values.cameraPosition = Camera::getInstance().getPosition().getXMVector();
	model_cb_values.WorldViewMatrix = (*data->world) * (*data->view);

	//render lights
	if (!m_unlit)
	{
		m_pLightManager->renderLights(*data->world, &model_cb_values);
	}
	else
	{
		model_cb_values.ambient_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//model_cb_values.ambient_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

	//material stuff
	if (m_pMaterial)
	{
		if (m_pTexture)
		{
			m_pMaterial->useTexture = true;
		}
		else
		{
			m_pMaterial->useTexture = false;
		}
		model_cb_values.material = *m_pMaterial;
	}
	//update 
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);

	//Vertex shader c buffer
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	
	//set pixel shader res
	//set pixel shader smapler
	if (m_pTexture)
	{
		m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);
		m_pImmediateContext->PSSetShaderResources(0, 1, &data->skyboxTexture);
		m_pImmediateContext->PSSetShaderResources(1, 1, &m_pTexture);
		
	}

	//draw
	m_pObject->Draw();
	
}

void TestModel::LookAt_XZ(float x, float z)
{

	float	dx = m_x - x,
			dz = m_z - z;

	m_yAngle = atan2(dx, dz) * (180.0f / XM_PI);
}

void TestModel::MoveForward(float distance)
{
	m_x += sin(m_yAngle * (XM_PI / 180.0f)) * distance;
	m_z += cos(m_yAngle * (XM_PI / 180.0f)) * distance;
}

void TestModel::CalculateModelCenterPoint()
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
	m_boundingSphereCenterX = minX + ((maxX - minX) / 2);
	m_boundingSphereCenterY = minY + ((maxY - minY) / 2);
	m_boundingSphereCenterZ = minZ + ((maxZ - minZ) / 2);
}

void TestModel::CalculateBoundingSphereRadius()
{
	m_boundingSphereRadius = 0;
	float	dx, dy, dz;
	for (unsigned int i = 0; i < m_pObject->numverts; i++)
	{
		dx = pow(m_pObject->vertices[i].Pos.x - m_boundingSphereCenterX, 2);
		dy = pow(m_pObject->vertices[i].Pos.y - m_boundingSphereCenterY, 2);
		dz = pow(m_pObject->vertices[i].Pos.z - m_boundingSphereCenterZ, 2);
		if ((dx + dy + dz) > m_boundingSphereRadius)
			m_boundingSphereRadius = (dx + dy + dz);
	}
	m_boundingSphereRadius = sqrtf(m_boundingSphereRadius);// = sqrtf(m_boundingSphereRadius);
}

XMVECTOR TestModel::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world;
	world = XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	XMVECTOR offset;
	offset = XMVectorSet(m_boundingSphereCenterX, m_boundingSphereCenterY, m_boundingSphereCenterZ, 0.0f);
	offset = XMVector3Transform(offset, world);
	return offset;
}

float TestModel::GetBoundingSphereRadius()
{
	//float result = sqrtf(m_boundingSphereRadius);//r2=9, result=3, =6
	return m_boundingSphereRadius;// *= m_scale;
}

float TestModel::GetBoundingSphereRadiusSqr()
{
	return m_boundingSphereRadius;// *= (m_scale*m_scale); //r2=9, 18
}

bool TestModel::checkCollision(TestModel* model)
{
	if (model == this) return false;

	float distanceSquared = (pow((m_x - model->getXPos()), 2) + pow((m_y - model->getYPos()), 2) + pow((m_z - model->getZPos()), 2));

	if (distanceSquared < pow(m_boundingSphereRadius + model->GetBoundingSphereRadius(), 2))
	{
		return true;
	}
	else
	{
		return false;
	}
}