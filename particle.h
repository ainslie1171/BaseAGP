#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "gameObject.h"

class Particle
{
protected:
	Vector3 m_position;
	Vector3 m_velocity;
	float m_life;
	float m_scale;
	GameObject* m_parent;
	ShaderManager* m_pShaderManager;
public:
	Particle(Vector3 position, Vector3 velocity, float life, GameObject* parent, ShaderManager* shaderManager);
	~Particle();
	void Update(float deltaTime);
	void Render(RENDER_DESC& desc);
	void Die(void);
};

#endif//_PARTICLE_H_

/*
Change Log:

*/