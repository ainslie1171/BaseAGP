#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT


#include <d3d11.h>
#include <xnamath.h>
#include <vector>

struct Light
{
	/////////////////////////////////////////////////////
	XMVECTOR Position;					//  16 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	XMVECTOR Direction;					//  16 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	XMVECTOR Colour;					//  16 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	float Angle;						//   4 bytes   //
	float constAttenuation;				//   4 bytes   //
	float linAttenuation;				//   4 bytes   //
	float quadAttenuation;				//   4 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	int type;							//    4 bytes  //
	int padding[3];						//   12 bytes  //
	//////////////////////16 bytes///////////////////////

	Light()
	{
		Position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		Direction = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		Colour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		Angle = 0.0f;
		constAttenuation = 0.0f;
		linAttenuation = 0.0f;
		quadAttenuation = 0.0f;
		type = 0;
	}

	Light(XMVECTOR p, XMVECTOR d, XMVECTOR col, float a, float cAttenuation, float lAttenuation, float qAttenuation, int t)
	{
		Position = p;
		Direction = d;
		Colour = col;
		Angle = a;
		constAttenuation = cAttenuation;
		linAttenuation = lAttenuation;
		quadAttenuation = qAttenuation;
		type = t;
	};
};

#include "constantBuffer0.h"

struct MODEL_CONSTANT_BUFFER;
struct REFLECT_CONSTANT_BUFFER;
struct TEST_CONSTANT_BUFFER;

enum LightType
{
	DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT
};

//TODO convert light manager from being a a singleton

class LightManager
{
private:
	std::vector<Light*> m_lights;
	XMVECTOR m_ambientLight;
public:
	LightManager();
	~LightManager();
	void addLight(const Light& light);
	void renderLights(const XMMATRIX& modelMatrix, MODEL_CONSTANT_BUFFER* cb0);
	void renderLights(const XMMATRIX& modelMatrix, REFLECT_CONSTANT_BUFFER* cb0);
	void renderLights(const XMMATRIX& modelMatrix, TEST_CONSTANT_BUFFER* cb0);
	void setAmbientLight(XMVECTOR colour);
	void removeLight(int index);
};
#endif