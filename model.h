#ifndef _MODEL_H_
#define _MODEL_H_

#include "objfilemodel.h"
#include "lightManager.h"
#include "camera.h"
#include "materialManager.h"
#include "shader.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

class Model
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	ObjFileModel* m_pObject;
	Shader*		m_pShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampler;
	Material*	m_pMaterial;
	
	//TODO clean up member variables in the model class
	float m_x, m_y, m_z,
		m_xAngle, m_yAngle, m_zAngle,
		m_scale,
		m_boundingSphereCenterX,
		m_boundingSphereCenterY,
		m_boundingSphereCenterZ,
		m_boundingSphereRadius;
	bool m_unlit;

	void CalculateModelCenterPoint();
	void CalculateBoundingSphereRadius();
public:
	Model(ID3D11Device* device, ID3D11DeviceContext* context);
	~Model();
	int LoadObjModel(char* fileName);
	//Don't worry, this class is going to be re-made
	void Draw(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, LightManager* lightManager);
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
	bool checkCollision(Model* model);
	ObjFileModel* getObject(){ return m_pObject; }
	void toggleUnlit(bool t){ m_unlit = t; }
};

#endif	_MODEL_H_