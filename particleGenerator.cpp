#include "particleGenerator.h"

ParticleGenerator::ParticleGenerator(const PARTICLE_GENERATOR_DESC& desc)
{
	m_pD3DDevice = desc.device;
	m_pImmediateContext = desc.context;
	m_pShaderManager = desc.shaderManager;
	m_shaderID = desc.targetShader;
	init();

	for (UINT i = 0; i < 100; i++)
	{
		Particle* p = new Particle();
		m_free.push_back(p);
	}

	m_particleSpawnRate = desc.particleSpawnRate;
	m_untilParticle = desc.particleSpawnRate;
}

//13
ParticleGenerator::~ParticleGenerator()
{
	while (!m_active.empty())
	{
		delete m_active.back();
		m_active.pop_back();
	}

	while (!m_free.empty())
	{
		delete m_free.back();
		m_free.pop_back();
	}

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
	
	
	float deltaTime = desc.deltaTime;
	m_untilParticle -= deltaTime;


	if (m_untilParticle <= 0 && !m_free.empty())
	{
		Particle* p = m_free.front();
		//set p values
		p->colour = { randomZeroToOne(), randomZeroToOne(), randomZeroToOne(), 1.0f };
		p->velocity = { randomNegOneToPosOne(), randomNegOneToPosOne(), randomNegOneToPosOne() };
		p->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		p->scale = 0.3f;
		p->mass = 0.3f;
		m_active.push_back(p);
		m_free.pop_front();
		m_untilParticle = m_particleSpawnRate;
	}

	list<Particle*>::iterator it = m_active.begin();
	list<list<Particle*>::iterator> dirty;
	while (it != m_active.end())
	{
		Particle* p = (*it);
		Vector3 force = ApplyForce(*p, gravity);
		Vector3 accel = CalcAcceleration(*p, force);
		p->velocity += accel * deltaTime;
		Vector3 Destination = p->position + (p->velocity * deltaTime);

		move(*p, Destination);
		drawOne(desc, *p);
		if (p->position.y <= -4.0f)
		{
			dirty.push_back(it);
		}
		it++;
	}

	for (list<Particle*>::iterator dead : dirty)
	{
		m_free.push_back(*dead);
		m_active.erase(dead);
	}
	/*
	Particle Test;
	Test.colour = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	Test.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	drawOne(desc, Test);
	*/
}

void ParticleGenerator::move(Particle& particle, const Vector4& destination)
{
	particle.position = XMFLOAT3(destination.x, destination.y, destination.z);
	if(checkCollision(particle, destination))
		particle.position = XMFLOAT3(-destination.x, -destination.y, -destination.z);
}

bool ParticleGenerator::checkCollision(const Particle& particle, const Vector4& destination)
{
	for (Particle* p : m_active)
	{
		if (p != &particle)
		{
			float distanceSq = distanceBetweenVectorsSqr(destination, p->position);
			float combRadiSq = (p->scale) + (particle.scale);
			combRadiSq *= combRadiSq;
			if (distanceSq < combRadiSq)
				return true;
		}
	}
	return false;
}

void ParticleGenerator::drawOne(RENDER_DESC& desc, const Particle& p)
{
	XMMATRIX world = XMMatrixIdentity();
	XMVECTOR d;
	d.x = p.position.x - desc.camera->x;
	d.y = p.position.y - desc.camera->y;
	d.z = p.position.z - desc.camera->z;

	float m_yAngle = atan2(d.x, d.z) + XM_PI;
	float dyz = d.z / cos(m_yAngle);
	float m_xAnlge = atan2(-d.y, dyz) + XM_PI;

	world = XMMatrixScaling(p.scale, p.scale, p.scale);
	world *= XMMatrixRotationX(m_xAnlge);
	world *= XMMatrixRotationY(m_yAngle);
	world *= XMMatrixTranslation(p.position.x, p.position.y, p.position.z);
	world *= (*desc.world);

	//create constatnt buffer
	PARTICLE_CONSTANT_BUFFER particle_cb_values;
	ZeroMemory(&particle_cb_values, sizeof(PARTICLE_CONSTANT_BUFFER));
	//add world view projection
	particle_cb_values.WorldViewProjection = (world)*(*desc.view)*(*desc.projection);
	particle_cb_values.colour = p.colour.getXMFloat4();
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

float ParticleGenerator::randomZeroToOne()
{
	float output = float(rand() % 101);
	return output / 100.0f;
}

float ParticleGenerator::randomNegOneToPosOne()
{
	float output = float(rand() % 201 - 100);
	return output / 100.0f;
}