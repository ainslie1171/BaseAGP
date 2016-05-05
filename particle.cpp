#include "particle.h"
Particle::Particle(Vector3 position, Vector3 velocity, float life, GameObject* parent, ShaderManager* shaderManager)
{
	m_position = position;
	m_velocity = velocity;
	m_life = life;
	m_parent = parent;
	m_pShaderManager = shaderManager;
}

Particle::~Particle()
{

}

void Particle::Update(float deltaTime)
{
	if (m_life > 0.0f)
	{
		m_position += m_velocity * deltaTime;
		m_life -= deltaTime;
	}
	else
	{
		Die();
	}
}

void Particle::Render(RENDER_DESC& desc)
{
	if (3 != desc.targetShader)
	{
		m_pShaderManager->updateShader(3);
		desc.targetShader = 3;
	}

	XMMATRIX world = XMMatrixIdentity();
	XMVECTOR d;
	d.x = m_position.x - desc.camera->x;
	d.y = m_position.y - desc.camera->y;
	d.z = m_position.z - desc.camera->z;

	float m_yAngle = atan2(d.x, d.z) + XM_PI;
	float dyz = d.z / cos(m_yAngle);
	float m_xAnlge = atan2(-d.y, dyz) + XM_PI;

	world = XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixRotationX(m_xAnlge);
	world *= XMMatrixRotationY(m_yAngle);
	world *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
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
	m_pShaderManager->getShader(3)->ImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pImmediateContext->Draw(6, 0);
}

void Particle::Die()
{
	//m_parent->removeParticle(this);
	this->~Particle();
}