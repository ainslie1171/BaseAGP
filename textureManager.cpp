#include "textureManager.h"

TextureManager::TextureManager(ID3D11Device* device)
{
	m_pDevice = device;
}

TextureManager::~TextureManager()
{
	while (!m_textures.empty())
	{
		delete m_textures.back();
		m_textures.pop_back();
	}
}

void TextureManager::add(char* filePath)
{
	Texture* t = new Texture();
	init(t, filePath);

	if (m_textures.size() < 1)
	{
		t->ID = 1;
	}
	else
	{
		t->ID = m_textures.back()->ID + 1;
	}

	m_textures.push_back(t);
}

HRESULT TextureManager::init(Texture* texture, char* filePath)
{
	HRESULT hr = S_OK;
	hr = D3DX11CreateShaderResourceViewFromFile(m_pDevice, filePath, NULL, NULL, &texture->m_pTexture, NULL);

	if (FAILED(hr))
		return hr;


	//Craete the Sampler State
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pDevice->CreateSamplerState(&sampler_desc, &texture->m_pSampler);

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void TextureManager::remove(unsigned int ID)
{
	for (Texture* t : m_textures)
	{
		if (t->ID == ID)
		{
			remove(t);
		}
	}
}

void TextureManager::remove(Texture* texture)
{
	std::vector<Texture*>::iterator it;
	it = std::find(m_textures.begin(), m_textures.end(), texture);
	m_textures.erase(it);
}

bool TextureManager::getTexture(unsigned int ID, Texture* textureOut)
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i]->ID == ID)
		{
			*textureOut = *m_textures[i];
			return true;
		}
	}
	return false;
}
