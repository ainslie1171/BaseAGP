#ifndef _SHADER_H_
#define _SHADER_H_

#include <d3d11.h>
#include <d3dx11.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

class Shader
{
private:
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	HRESULT init(ID3D11Device* device, char* filePath);
public:
	Shader(ID3D11Device* device, char* filePath);
	~Shader();
	ID3D11VertexShader* getVertexShader() { return m_pVShader; }
	ID3D11PixelShader* getPixelShader() { return m_pPShader; }
	ID3D11InputLayout* getInputLayout() { return m_pInputLayout; }
};

#endif