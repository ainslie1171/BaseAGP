#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;

struct Texture
{
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampler;
	unsigned int ID;
};

class TextureManager
{
private:
	ID3D11Device* m_pDevice;
	std::vector<Texture*> m_textures;

	HRESULT init(Texture* texture, char* filePath);
public:
	TextureManager(ID3D11Device* device);
	~TextureManager();
	void add(char* filePath);
	void remove(Texture* texture);
	void remove(unsigned int ID);
	bool getTexture(unsigned int ID, Texture* textureOut);
};

#endif