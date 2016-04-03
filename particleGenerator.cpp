#include "particleGenerator.h"

ParticleGenerator::ParticleGenerator(const PARTICLE_GENERATOR_DESC& desc)
{
	m_pD3DDevice = desc.device;
	m_pImmediateContext = desc.context;
	m_pShaderManager = desc.shaderManager;
	m_shaderID = desc.targetShader;
	init();
}

//13
ParticleGenerator::~ParticleGenerator()
{
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
}

HRESULT ParticleGenerator::init()
{
	HRESULT hr = S_OK;
	const unsigned int VertCount = 6;
	XMFLOAT3 vertices[VertCount] = 
	{
		XMFLOAT3(-1.0f, -1.0f, 0.0f),
		XMFLOAT3(1.0f, 1.0f, 0.0f),
		XMFLOAT3(-1.0f, 1.0f, 0.0f),
		XMFLOAT3(-1.0f, -1.0f, 0.0f),
		XMFLOAT3(1.0f, -1.0f, 0.0f),
		XMFLOAT3(1.0f, 1.0f, 0.0f),
	};

	// setup the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //Can use UpdateSubresourse() to update
	constant_buffer_desc.ByteWidth = sizeof(PARTICLE_CONSTANT_BUFFER); //MUST be a multiple of 16, calculated from the constant buffer struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //Use as a constant buffer
	//create the constant buffer
	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);

	if (FAILED(hr))
		return hr;

	// setup the constant buffer
	D3D11_BUFFER_DESC vertex_buffer_desc;
	ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC; //Can use UpdateSubresourse() to update
	vertex_buffer_desc.ByteWidth = sizeof(XMFLOAT3)*VertCount; //MUST be a multiple of 16, calculated from the constant buffer struct
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //Use as a constant buffer
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//create the constant buffer
	hr = m_pD3DDevice->CreateBuffer(&vertex_buffer_desc, NULL, &m_pVertexBuffer);

	if (FAILED(hr))
		return hr;

	D3D11_MAPPED_SUBRESOURCE ms;

	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));
	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);

	return S_OK;
}

void ParticleGenerator::draw(RENDER_DESC& desc)
{
	if (m_shaderID != desc.targetShader) updateShader(desc);

	Particle test;
	test.colour = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	test.gravity = 1.0f;
	test.position = XMFLOAT3(0.0f, 0.0f, 3.0f);
	test.velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	drawOne(desc, test);

}

void ParticleGenerator::drawOne(RENDER_DESC& desc, const Particle& p)
{
	XMMATRIX world = XMMatrixIdentity();
	world = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	world *= XMMatrixRotationY(XM_PI);
	world *= XMMatrixTranslation(p.position.x, p.position.y, p.position.z);
	world *= (*desc.world);

	//create constatnt buffer
	PARTICLE_CONSTANT_BUFFER particle_cb_values;
	ZeroMemory(&particle_cb_values, sizeof(PARTICLE_CONSTANT_BUFFER));
	//add world view projection
	particle_cb_values.WorldViewProjection = (world)*(*desc.view)*(*desc.projection);
	particle_cb_values.colour = p.colour;
	//update 
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &particle_cb_values, 0, 0);
	//Vertex shader c buffer
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pImmediateContext->Draw(6, 0);
}

void ParticleGenerator::updateShader(RENDER_DESC& desc)
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
