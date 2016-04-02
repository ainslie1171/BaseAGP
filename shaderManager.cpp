#include "shaderManager.h"

ShaderManager::ShaderManager(ID3D11Device* device)
{
	m_pDevice = device;
}

ShaderManager::~ShaderManager()
{
	//TODO delete shaders
	while (!m_shaders.empty())
	{
		delete m_shaders.back();
		m_shaders.pop_back();
	}
}

void ShaderManager::add(char* filePath)
{
	Shader* s = new Shader();
	init(s, filePath);
	
	if (m_shaders.size() < 1)
	{
		s->ID = 1;
	}
	else
	{
		s->ID = m_shaders.back()->ID + 1;
	}

	m_shaders.push_back(s);
}

HRESULT ShaderManager::init(Shader* shader, char* filePath)
{
	HRESULT hr = S_OK;

	//Load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS,
		*PS,
		*error;
	hr = D3DX11CompileFromFile(filePath, 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	//check for shader compilation error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//don't fail if error is just a warning
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile(filePath, 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	//check for shader compilation error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//don't fail if error is just a warning
		{
			return hr;
		};
	}

	//Create shader objects
	hr = m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &shader->VShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &shader->PShader);

	if (FAILED(hr))
	{
		return hr;
	}

	//Create and set the input layout object
	//TODO might have to change this at a later date
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = m_pDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &shader->InputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void ShaderManager::remove(unsigned int ID)
{
	for (Shader* s : m_shaders)
	{
		if (s->ID == ID)
		{
			remove(s);
		}
	}
}

void ShaderManager::remove(Shader* shader)
{
	std::vector<Shader*>::iterator it;
	it = std::find(m_shaders.begin(), m_shaders.end(), shader);
	m_shaders.erase(it);
}

bool ShaderManager::getShader(unsigned int ID, Shader* returnedShader)
{
	for (unsigned int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i]->ID == ID)
		{
			*returnedShader = *m_shaders[i];
			return true;
		}
	}
	return false;
}
