#ifndef _TEST_MODEL_H_
#define _TEST_MODEL_H_

#include "lightManager.h"
#include "camera.h"
#include "objfilemodel.h"
#include "constantBuffer0.h"
#include "shaderManager.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;

struct RenderInfo
{
	XMMATRIX* world;
	XMMATRIX* view;
	XMMATRIX* projection;
	ID3D11ShaderResourceView* skyboxTexture;
};

struct testModelDesc
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	LightManager* lightManager;
	ShaderManager* shaderManager;
};

class TestModel
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11Buffer* m_pConstantBuffer;
	
	ObjFileModel* m_pObject;
	Material*	m_pMaterial;
		
	LightManager* m_pLightManager;
	ShaderManager* m_pShaderManager;
	
	XMMATRIX m_transform;

	static const XMVECTOR xAxis;
	static const XMVECTOR yAxis;
	static const XMVECTOR zAxis;

	float m_x, m_y, m_z,
		m_xAngle, m_yAngle, m_zAngle,
		m_scale,
		m_boundingSphereCenterX,
		m_boundingSphereCenterY,
		m_boundingSphereCenterZ,
		m_boundingSphereRadius;
	bool m_unlit;

	unsigned int m_shaderID;

	void CalculateModelCenterPoint();
	void CalculateBoundingSphereRadius();

public:
	TestModel(const testModelDesc& desc);
	~TestModel();
	int LoadObjModel(char* fileName);
	//Don't worry, this class is going to be re-made
	void Draw(RenderInfo* data);
	int AddTexture(char* fileName);
	void setMaterial(Material* material){ m_pMaterial = material; }
	void setXPos(float XPos){ m_x = XPos; }
	void setYPos(float YPos){ m_y = YPos; }
	void setZPos(float ZPos){ m_z = ZPos; }
	void setXAngle(float XAngle){ m_xAngle = XAngle; }
	void setYAngle(float YAngle){ m_yAngle = YAngle; }
	void setZAngle(float ZAngle){ m_zAngle = ZAngle; }
	void setScale(float scale){ m_scale = scale; }
	float getXPos() { return m_x; }
	float getYPos() { return m_y; }
	float getZPos() { return m_z; }
	float getXAngle() { return m_xAngle; }
	float getYAngle() { return m_yAngle; }
	float getZAngle() { return m_zAngle; }
	float getScale() { return m_scale; }
	void IncXPos(float inc){ m_x += inc; }
	void IncYPos(float inc){ m_y += inc; }
	void IncZPos(float inc){ m_z += inc; }
	void IncXAngle(float inc){ m_xAngle += inc; }
	void IncYAngle(float inc){ m_yAngle += inc; }
	void IncZAngle(float inc){ m_zAngle += inc; }
	void IncScale(float inc){ m_scale += inc; }
	void LookAt_XZ(float x, float z);
	void MoveForward(float distance);
	XMVECTOR GetBoundingSphereWorldSpacePosition();
	float GetBoundingSphereRadius();
	float GetBoundingSphereRadiusSqr();
	bool checkCollision(TestModel* model);
	ObjFileModel* getObject(){ return m_pObject; }
	void toggleUnlit(bool t){ m_unlit = t; }
	unsigned int getShaderID(){ return m_shaderID; }
};

#endif	_MODEL_H_