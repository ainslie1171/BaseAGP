#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_


#include <list>
#include "vector.h"
class GameObject;
struct RENDER_DESC;

struct ID3D11Device;
struct ID3D11DeviceContext;
class ShaderManager;
struct ID3D11Buffer;


struct Particle
{
	Vector3 position;
	Vector3 velocity;
	float life;
	float scale;
	GameObject* parent;
	bool dirty;
};

struct PARTICLE_MANAGER_DESC
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ShaderManager* shaderManager;
	unsigned int maxNumParticles;
};

class ParticleManager
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11Buffer* m_pVertexBuffer;
	ShaderManager* m_pShaderManager;


	std::list<Particle*> m_free;
	std::list<Particle*> m_active;
public:
	ParticleManager(const PARTICLE_MANAGER_DESC& desc);
	~ParticleManager();
	void spawnParticle(Vector3 position, Vector3 velocity, float life, GameObject* parent = nullptr, float scale = 1.0f);
	Particle* spawnParticleRef(Vector3 position, Vector3 velocity, float life, GameObject* parent = nullptr, float scale = 1.0f);
	void Render(RENDER_DESC& desc);
	void Update(float deltaTime);
};

#endif//_PARTICLEMANAGER_H_

/*
Change Log:

*/