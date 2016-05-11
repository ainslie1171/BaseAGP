#ifndef _CONSTANT_BUFFER_0_
#define _CONSTANT_BUFFER_0_


#define MAX_LIGHTS 10

#include "lightManager.h"
#include "materialManager.h"


struct MODEL_CONSTANT_BUFFER
{
	/////////////////////////////////////////////////
	XMMATRIX WorldMatrix;			//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX InvTransWorldMatrix;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMVECTOR cameraPosition;		//   16 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	XMVECTOR ambient_light_colour;	//   16 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	Light lights[MAX_LIGHTS];		//  800 bytes  //
	/////////////////10 x 80 bytes///////////////////

	/////////////////////////////////////////////////
	int lightCount;					//    4 bytes  //
	int padding[3];					//   12 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	Material material;				//   80 bytes  //
	////////////////////80 bytes/////////////////////
};

struct REFLECT_CONSTANT_BUFFER
{
	/////////////////////////////////////////////////
	XMMATRIX WorldMatrix;			//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX WorldViewMatrix;		//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX InvTransWorldMatrix;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMVECTOR cameraPosition;		//   16 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	XMVECTOR ambient_light_colour;	//   16 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	Light lights[MAX_LIGHTS];		//  800 bytes  //
	/////////////////10 x 80 bytes///////////////////

	/////////////////////////////////////////////////
	int lightCount;					//    4 bytes  //
	int padding[3];					//   12 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	Material material;				//   80 bytes  //
	////////////////////80 bytes/////////////////////
};

struct TEST_CONSTANT_BUFFER
{
	/////////////////////////////////////////////////
	XMMATRIX WorldMatrix;			//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX WorldViewMatrix;		//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX InvTransWorldMatrix;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMVECTOR cameraPosition;		//   16 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	XMVECTOR ambient_light_colour;	//   16 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	Light lights[MAX_LIGHTS];		//  800 bytes  //
	/////////////////10 x 80 bytes///////////////////

	/////////////////////////////////////////////////
	int lightCount;					//    4 bytes  //
	int padding[3];					//   12 bytes  //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	Material material;				//   80 bytes  //
	////////////////////80 bytes/////////////////////
};

struct SKYBOX_CONSTANT_BUFFER
{
	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////
};

struct PARTICLE_CONSTANT_BUFFER2
{
	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//   64 bytes  //
	//////////////////4 x 16 bytes///////////////////

	/////////////////////////////////////////////////
	XMFLOAT4 colour;				//   16 bytes  //
	////////////////////16 bytes/////////////////////
};
#endif