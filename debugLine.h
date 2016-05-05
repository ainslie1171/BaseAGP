#ifndef _DEBUGLINE_H_
#define _DEBUGLINE_H_

#include"skybox.h"
#include "vector.h"


struct CONSTANT_BUFFER
{
	/////////////////////////////////////////////////
	XMFLOAT4 colour;				//   16 bytes   //
	////////////////////16 bytes/////////////////////

	/////////////////////////////////////////////////
	XMMATRIX WorldViewProjection;	//  64 bytes   //
	//////////////////3 x 16 bytes///////////////////
};


class DebugLine
{
private:
	static DebugLine* m_pInstance;
	ID3D11Device*           pD3DDevice;
	ID3D11DeviceContext*    pImmediateContext;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer*			pConstantBuffer;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	CONSTANT_BUFFER cbValues;
	Vector3 vertices[3];
	
	DebugLine(void){};

	
public:
	static DebugLine* getInstance(void);
	void Init(ID3D11Device* device, ID3D11DeviceContext* context);
	~DebugLine();
	void draw(Vector3 PosA, Vector3 PosB, Vector3 col = {0.0f, 1.0f, 0.0f});
	void Render(XMMATRIX wvp);
};

#endif//_DEBUGLINE_H_

/*
Change Log:

*/