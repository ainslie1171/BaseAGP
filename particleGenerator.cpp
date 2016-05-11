#include "particleGenerator.h"


ParticleGenerator::ParticleGenerator(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_pD3DDevice = device;
	m_pImmediateContext = context;

	init();

	for (UINT i = 0; i < 1000; i++)
	{
		p_Particle* p = new p_Particle();
		m_free.push_back(p);
	}
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







void ParticleGenerator::drawOne(RENDER_DESC2& desc, const p_Particle& p)
{
	XMMATRIX world = XMMatrixIdentity();
	XMVECTOR d;
	d.x = p.Position.x - desc.camera->x;
	d.y = p.Position.y - desc.camera->y;
	d.z = p.Position.z - desc.camera->z;

	float m_yAngle = atan2(d.x, d.z) + XM_PI;
	float dyz = d.z / cos(m_yAngle);
	float m_xAnlge = atan2(-d.y, dyz) + XM_PI;

	world = XMMatrixScaling(p.scale, p.scale, p.scale);
	world *= XMMatrixRotationX(m_xAnlge);
	world *= XMMatrixRotationY(m_yAngle);
	world *= XMMatrixTranslation(p.Position.x, p.Position.y, p.Position.z);
	world *= (*desc.world);

	//create constatnt buffer
	PARTICLE_CONSTANT_BUFFER particle_cb_values;
	ZeroMemory(&particle_cb_values, sizeof(PARTICLE_CONSTANT_BUFFER));
	//add world view projection
	particle_cb_values.WorldViewProjection = (world)*(*desc.view)*(*desc.projection);
	particle_cb_values.colour = p.c.getXMFloat4();
	//update 
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &particle_cb_values, 0, 0);
	//Vertex shader c buffer
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pImmediateContext->Draw(6, 0);
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






void ParticleGenerator::create()
{
	p_Particle* p = m_free.front();
	p->Position = { 0.0f, 3.0f, 10.0f };
	p->Velocity = { 0.5f, 0.0f, 0.0f };
	p->scale = 0.1f;
	p->Mass = 0.1f;
	p->InvMass = 1 / p->Mass;
	p->time = 0.0f;
	p->checkColl = false;
	p->c = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_active.push_back(p);
	m_free.pop_front();

	p = m_free.front();
	p->Position = { 3.0f, 3.0f, 10.0f };
	p->Velocity = ZeroVector3;
	p->scale = 0.1f;
	p->Mass = 0.1f;
	p->InvMass = 1 / p->Mass;
	p->time = 0.0f;
	p->checkColl = false;
	p->c = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	m_active.push_back(p);
	m_free.pop_front();

	p = m_free.front();
	p->Position = { 1.5f, 0.0f, 10.0f };
	p->Velocity = {0.0f, 1.0f, 0.0f};
	p->scale = 0.1f;
	p->Mass = 0.1f;
	p->InvMass = 1 / p->Mass;
	p->time = 0.0f;
	p->checkColl = false;
	p->c = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_active.push_back(p);
	m_free.pop_front();
	

}

void ParticleGenerator::drawP(RENDER_DESC2& desc)
{

	p_Particle Test;
	Test.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Test.scale = 0.1f;
	Test.c = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	drawOne(desc, Test);

	//update(dt);
	for (p_Particle* p : m_active)
	{
		drawOne(desc, *p);
	}

	
}

void ParticleGenerator::update(float dt)
{
	for (p_Particle* p : m_active)
	{
		p->applyForce(gravity, dt);
	}

	std::list<p_Particle*>::iterator j, i = m_active.begin();
	p_Particle *p1, *p2;
	while (i != m_active.end())
	{
		j = i;
		j++;
		p1 = (*i);
		p1->betterCheckBoundries(dt);
		while (j != m_active.end())
		{
			p2 = (*j);
			//check collision
			if (p1->betterCollisionCheck(*p2, dt))
			{
				//resolve collision
				p1->collisionResponse(*p2);
			}
			j++;
		}
		
		i++;
	}

	for (p_Particle* p : m_active)
	{
		p->stepPosition(dt);
	}
}

void ParticleGenerator::spawnParticle()
{
	if (!m_free.empty())
	{
		p_Particle* p = m_free.front();
		//set p values
		p->Velocity = { randomNegOneToPosOne(), randomZeroToOne(), randomNegOneToPosOne() };
		p->Position = { randomNegOneToPosOne() * 3.0f, randomZeroToOne() * 3.0f, randomNegOneToPosOne() * 3.0f };
		p->scale = 0.3f;
		p->Mass = 1.0f;
		p->InvMass = 1 / p->Mass;
		p->time = 0.0f;
		p->checkColl = false;
		p->c = Vector4(randomZeroToOne(), randomZeroToOne(), randomZeroToOne(), 1.0f);
		bool result = false;
		for(p_Particle* Pi : m_active)
		{
			if (p->betterCollisionCheck(*Pi, 1.0f / 60.0f)) result = true;
		}
		while (result)
		{
			result = false;
			p->Position = { randomNegOneToPosOne() * 3.0f, randomZeroToOne() * 3.0f, randomNegOneToPosOne() * 3.0f };
			for (p_Particle* Pi : m_active)
			{
				if (p->betterCollisionCheck(*Pi, 1.0f / 60.0f)) result = true;
			}
		} 

		m_active.push_back(p);
		m_free.pop_front();
	}
}