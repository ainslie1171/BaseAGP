#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "model.h"
#include "constantBuffer0.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;

typedef struct SKYBOX_DESC
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	char* textureFilepath;
}SKYBOX_DESC;

class Skybox
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	Model* m_pModel;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampler;

public:
	Skybox(const SKYBOX_DESC&);
	~Skybox();

	int LoadObjModel();
	void Draw(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection);
	int setTexture(char* fileName);
	ID3D11ShaderResourceView* getTexture(){ return m_pTexture; }
};

#endif