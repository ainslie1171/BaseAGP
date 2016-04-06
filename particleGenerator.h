#ifndef _PARTICLE_GENERATOR_H_
#define _PARTICLE_GENERATOR_H_

#include "model.h"
#include <list>
#include "physics.h"


struct Particle
{
	Vector3 position;
	Vector3 linVelocity;
	Vector4 colour;
	float mass;
	float radius;
	float angle;
	float angVelocity;

};

struct PARTICLE_GENERATOR_DESC
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ShaderManager* shaderManager;
	UINT targetShader;
	float particleSpawnRate;
};

class ParticleGenerator
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;
	ShaderManager* m_pShaderManager;
	UINT m_shaderID;
	float m_untilParticle;
	float m_particleSpawnRate;
	std::list<p_Particle*> m_free;
	std::list<p_Particle*> m_active;

	HRESULT init();
	void updateShader(RENDER_DESC& desc);
	void drawOne(RENDER_DESC& desc, const p_Particle& p);
	float randomZeroToOne();
	float randomNegOneToPosOne();
	void move(p_Particle& particle, const Vector4& destination);
	bool checkCollision(const p_Particle& particle, const Vector4& destination);
public:
	ParticleGenerator(const PARTICLE_GENERATOR_DESC& desc);
	~ParticleGenerator();
	void draw(RENDER_DESC& desc);
};

#endif
