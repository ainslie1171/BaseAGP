#ifndef _PARTICLE_GENERATOR_H_
#define _PARTICLE_GENERATOR_H_

#include <list>
#include <map>
#include "physics.h"

#include "shaderManager.h"

struct RENDER_DESC2
{
	XMVECTOR* camera;
	XMMATRIX* world;
	XMMATRIX* view;
	XMMATRIX* projection;
};

struct PARTICLE_CONSTANT_BUFFER
{
	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMFLOAT4 colour;				//   16 bytes  //
	////////////////////16 bytes/////////////////////
};


class ParticleGenerator
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;

	std::list<p_Particle*> m_free;
	std::list<p_Particle*> m_active;
	std::map<p_Particle*, p_Particle*> m_collisions;

	HRESULT init();

	void drawOne(RENDER_DESC2& desc, const p_Particle& p);

	float randomZeroToOne();
	float randomNegOneToPosOne();

public:
	ParticleGenerator(ID3D11Device* device, ID3D11DeviceContext* context);
	~ParticleGenerator();

	void create();
	void drawS(RENDER_DESC2& desc);
	void drawP(RENDER_DESC2& desc);
	void update(float dt);
	void spawnParticle();
};
#endif
