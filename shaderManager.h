#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

struct Shader
{
	ID3D11VertexShader* VShader;
	ID3D11PixelShader* PShader;
	ID3D11InputLayout* InputLayout;
	unsigned int ID;
};

class ShaderManager
{
private:
	ID3D11Device* m_pDevice;
	std::vector<Shader*> m_shaders;

	HRESULT init(Shader* shader, char* filePath);
public:
	ShaderManager(ID3D11Device* device);
	~ShaderManager();
	void add(char* filePath);
	void remove(Shader* shader);
	void remove(unsigned int ID);
	bool getShader(unsigned int ID, Shader* returnedShader);
};

#endif