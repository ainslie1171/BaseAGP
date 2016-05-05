#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <vector>

#include "constantBuffer0.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

struct Shader
{
	ID3D11DeviceContext* ImmediateContext;
	ID3D11VertexShader* VShader;
	ID3D11PixelShader* PShader;
	ID3D11InputLayout* InputLayout;
	ID3D11Buffer* constantBuffer;
	UINT ID;
	~Shader()
	{
		VShader->Release();
		PShader->Release();
		InputLayout->Release();
		constantBuffer->Release();
	}
	void setCBuffer(ID3D11Device* pDevice, UINT ConstantBufferSize)
	{
		HRESULT hr;
		// setup the constant buffer
		D3D11_BUFFER_DESC constant_buffer_desc;
		ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
		constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //Can use UpdateSubresourse() to update
		constant_buffer_desc.ByteWidth = ConstantBufferSize; //MUST be a multiple of 16, calculated from the constant buffer struct
		constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //Use as a constant buffer

		//create the constant buffer
		hr = pDevice->CreateBuffer(&constant_buffer_desc, NULL, &constantBuffer);
		if (FAILED(hr))
		{
			OutputDebugString("Failed to create shader's constant buffer");
		}
	}
	void updateResources(const void* cb, ID3D11ShaderResourceView* skybox)
	{
		//update 
		ImmediateContext->UpdateSubresource(constantBuffer, 0, 0, cb, 0, 0);

		//Vertex shader c buffer
		ImmediateContext->VSSetConstantBuffers(0, 1, &constantBuffer);
		ImmediateContext->PSSetConstantBuffers(0, 1, &constantBuffer);
		if (skybox)
		{
			//ImmediateContext->PSSetShaderResources(1, 1, &skybox);
		}
	}
};

struct InputElementWrapper
{
	D3D11_INPUT_ELEMENT_DESC iedesc;
};

class ShaderManager
{
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	std::vector<Shader*> m_shaders;

	HRESULT init(Shader* shader, char* filePath, int inputElementSize);
public:
	ShaderManager(ID3D11Device* device, ID3D11DeviceContext* context);
	~ShaderManager();
	void add(char* filePath, int inputElementSize);
	void remove(Shader* shader);
	void remove(UINT ID);
	bool getShader(UINT ID, Shader* returnedShader);
	Shader* getShader(UINT ID);
	bool setShader(UINT ID);
	void updateShader(UINT ID);
	void updateResources(const void* cb, ID3D11ShaderResourceView* skybox, UINT ShaderID);
};

#endif