#ifndef _MODEL_H_
#define _MODEL_H_

#include "objfilemodel.h"
#include "lightManager.h"
#include "camera.h"
#include "materialManager.h"
#include "shaderManager.h"
#include "textureManager.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

struct RENDER_DESC
{
	XMVECTOR* camera;
	XMMATRIX* world;
	XMMATRIX* view;
	XMMATRIX* projection;
	ID3D11ShaderResourceView* skyboxTexture;
	unsigned int targetShader;
	unsigned int targetTexture;
};

struct MODEL_DESC
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	LightManager* lightManager;
	ShaderManager* shaderManager;
	TextureManager* textureManager;
	char* filePath;
	unsigned int targetShader;
	unsigned int targetTexture;
	Material* material;
};

class Model
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11Buffer* m_pConstantBuffer;

	ObjFileModel* m_pObject;
	Material*	m_pMaterial;

	LightManager* m_pLightManager;
	ShaderManager* m_pShaderManager;
	TextureManager* m_pTextureManager;
	
	Vector4 m_boundingShpere;

	float m_scale;
	bool m_unlit;
	unsigned int m_shaderID;
	unsigned int m_textureID;

	void CalculateModelCenterPoint();
	void CalculateBoundingSphereRadius();
	int LoadObjModel(char* fileName);
	void updateShader(RENDER_DESC& desc);
	void updateTexture(RENDER_DESC& desc);

public:
	Model(const MODEL_DESC& desc);
	~Model();
	
	ObjFileModel* getObject(){ return m_pObject; }

	void Draw(RENDER_DESC& desc);

	void setMaterial(Material* material){ m_pMaterial = material; }

	void setScale(float scale){ m_scale = scale; }
	float getScale() { return m_scale; }

	float GetBoundingSphereRadius();
	float GetBoundingSphereRadiusSqr();

	unsigned int getShaderID(){ return m_shaderID; }
	unsigned int getTextureID(){ return m_textureID; }

	void toggleUnlit(bool t){ m_unlit = t; }

	
};

#endif	_MODEL_H_