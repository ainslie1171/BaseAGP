#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3dx11.h>
#include <xnamath.h>

//TODO POS_TEX_VERTEX
struct POS_TEX_COL_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT2 Texture;
	XMFLOAT4 Colour;
};

const int MAX_CHARACTERS = 10000;

class Object2D
{
private:

protected:
	//Attributes
	char* m_filename;
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	//might change this in the future
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampler;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;

	POS_TEX_COL_VERTEX m_pVertices[MAX_CHARACTERS * 6];
	int m_sizeOfVertexArr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11DepthStencilState* m_pDepthEnabledStencilState;
	ID3D11DepthStencilState* m_pDepthDisabledStencilState;

	//Methods
	void MapSubResouce(int size);
	void SetRenderStates(void);
public:
	Object2D(char* filename, ID3D11Device* device, ID3D11DeviceContext* context, int sizeofVertexArr);
	virtual ~Object2D(void);
	virtual void Render(void) = 0;
};

#endif//_OBJECT2D_H_

/*
Change Log:
-added Render as a pure virtual method as it should be contained by all children of this class
-changed the POS_TEX_VERTEX to include colour and made updates to the shader and the render function to make use of this

*/