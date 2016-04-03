#ifndef _PARTICLE_GENERATOR_H_
#define _PARTICLE_GENERATOR_H_

#include "model.h"

struct Particle
{
	float gravity;
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT4 colour;
};

struct PARTICLE_GENERATOR_DESC
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ShaderManager* shaderManager;
	UINT targetShader;
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

	HRESULT init();
	void updateShader(RENDER_DESC& desc);
	void drawOne(RENDER_DESC& desc, const Particle& p);
public:
	ParticleGenerator(const PARTICLE_GENERATOR_DESC& desc);
	~ParticleGenerator();
	void draw(RENDER_DESC& desc);
};

#endif
