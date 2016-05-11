#ifndef _PLANE_H_
#define _PLANE_H_

#include "vector.h"
#include <d3dx11.h>

struct PlaneBuffer
{
	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//  64 bytes   //
	//////////////////3 x 16 bytes///////////////////
};

struct POS_COL_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
};

class p_Plane
{
private:
	Vector3 position;
	Vector3 rotation;
	float scale;
	POS_COL_VERTEX m_pVertices[6];
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11DeviceContext*    m_pImmediateContext;
public:
	p_Plane(ID3D11Device* device, ID3D11DeviceContext* context, Vector3 pos, Vector3 rot, float s);
	~p_Plane();
	void draw(XMMATRIX vp);
	void setPos(Vector3 pos);
	void setRot(Vector3 rot);
};

#endif //_PLANE_H_