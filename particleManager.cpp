#include "particleManager.h"
#include "gameObject.h"

ParticleManager::ParticleManager(const PARTICLE_MANAGER_DESC& desc)
{
	m_pImmediateContext = desc.context;
	m_pD3DDevice = desc.device;
	m_pShaderManager = desc.shaderManager;
	
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
	D3D11_BUFFER_DESC vertex_buffer_desc;
	ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC; //Can use UpdateSubresourse() to update
	vertex_buffer_desc.ByteWidth = sizeof(XMFLOAT3)*VertCount; //MUST be a multiple of 16, calculated from the constant buffer struct
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //Use as a constant buffer
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//create the constant buffer
	m_pD3DDevice->CreateBuffer(&vertex_buffer_desc, NULL, &m_pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;

	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));
	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);

	for (UINT i = 0; i < desc.maxNumParticles; i++)
	{
		Particle* p = new Particle();
		m_free.push_back(p);
	}
}

ParticleManager::~ParticleManager()
{
	while (!m_active.empty()) delete m_active.back();
	while (!m_free.empty()) delete m_free.back();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
}
void ParticleManager::spawnParticle(Vector3 position, Vector3 velocity, float life, GameObject* parent, float scale)
{
	if (!m_free.empty())
	{
		Particle* p = m_free.front();
		p->velocity = velocity;
		p->position = position;
		p->life = life;
		p->scale = scale;
		p->parent = parent;
		p->dirty = false;
		m_active.push_back(p);
		m_free.pop_front();
	}
}

Particle* ParticleManager::spawnParticleRef(Vector3 position, Vector3 velocity, float life, GameObject* parent, float scale)
{
	if (!m_free.empty())
	{
		Particle* p = m_free.front();
		p->velocity = velocity;
		p->position = position;
		p->life = life;
		p->scale = scale;
		p->parent = parent;
		p->dirty = false;
		m_active.push_back(p);
		m_free.pop_front();
		return p;
	}
	return nullptr;
}


void ParticleManager::Render(RENDER_DESC& desc)
{
	for (Particle* p : m_active)
	{
		if (3 != desc.targetShader)
		{
			m_pShaderManager->updateShader(3);
			desc.targetShader = 3;
		}

		XMMATRIX world = XMMatrixIdentity();
		XMVECTOR d;
		d.x = p->position.x - desc.camera->x;
		d.y = p->position.y - desc.camera->y;
		d.z = p->position.z - desc.camera->z;

		float m_yAngle = atan2(d.x, d.z) + XM_PI;
		float dyz = d.z / cos(m_yAngle);
		float m_xAnlge = atan2(-d.y, dyz) + XM_PI;

		world = XMMatrixScaling(p->scale, p->scale, p->scale);
		world *= XMMatrixRotationX(m_xAnlge);
		world *= XMMatrixRotationY(m_yAngle);
		world *= XMMatrixTranslation(p->position.x, p->position.y, p->position.z);
		world *= (*desc.world);

		//create constatnt buffer
		PARTICLE_CONSTANT_BUFFER particle_cb_values;
		ZeroMemory(&particle_cb_values, sizeof(PARTICLE_CONSTANT_BUFFER));
		//add world view projection
		particle_cb_values.WorldViewProjection = (world)*(*desc.view)*(*desc.projection);
		particle_cb_values.colour = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		//update 
		m_pShaderManager->updateResources(&particle_cb_values, nullptr, 3);
		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		m_pImmediateContext->Draw(6, 0);
	}
}

void ParticleManager::Update(float deltaTime)
{
	list<Particle*>::iterator it = m_active.begin();
	list<list<Particle*>::iterator> forRemoval;
	for (Particle* p : m_active)
	{
		if (p->life > 0.0f)
		{
			p->position += p->velocity * deltaTime;
			p->life -= deltaTime;
		}
		else
		{
			p->dirty = true;
		}

		if (p->dirty)
		{
			forRemoval.push_back(it);
		}
		it++;
	}

	for (list<Particle*>::iterator p : forRemoval)
	{
		m_free.push_back(*p);
		m_active.erase(p);
	}
}