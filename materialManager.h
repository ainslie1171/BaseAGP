#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>
#include <vector>

using namespace std;

enum MATERIALS
{
	test,
	WhitePlastic,
	YellowPlastic,
	RedPlastic,
	Gold
};

struct Material
{
	/////////////////////////////////////////////////////
	XMVECTOR Emissive;					//  16 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	XMVECTOR Ambient;					//  16 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	XMVECTOR Diffuse;					//  16 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	XMVECTOR Specular;					//  16 bytes   //
	//////////////////////16 bytes///////////////////////

	/////////////////////////////////////////////////////
	float specularPower;				//   4 bytes   //
	int useTexture;						//   4 bytes   //
	int padding[2];						//   8 bytes   //
	//////////////////////16 bytes///////////////////////
};


class materialManager
{
private:
	vector<Material*> m_Materials;
public:
	materialManager();
	~materialManager();

	Material* getMaterial(unsigned int material){ return m_Materials[material]; };
};

#endif