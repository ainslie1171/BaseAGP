#ifndef _GAME_H_
#define _GAME_H_



#include "inputManager.h"
#include <d3dx11.h>
#include "text2D.h"
#include "camera.h"
#include "materialManager.h"
#include "lightManager.h"
#include "modelManager.h"
#include "maths.h"
#include "skybox.h"
#include "shaderManager.h"
#include "textureManager.h"
//#include "particleGenerator.h"
#include "particleManager.h"
#include "player.h"
#include "debugLine.h"

//Enemies
#include "groundEnemy.h"
#include "flyingEnemy.h"

class DX11Framework;

class Game
{
private:
	//InitialiseWindow
	HWND m_hWnd;
	HINSTANCE m_hInst;
	char m_TutorialName[100];

	//InitialiseD3D
	D3D_DRIVER_TYPE				m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL			m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*				m_pD3DDevice = NULL;
	ID3D11DeviceContext*		m_pImmediateContext = NULL;
	IDXGISwapChain*				m_pSwapChain = NULL;
	ID3D11RenderTargetView*		m_pBackBufferRTView = NULL;
	ID3D11DepthStencilView*		m_pZBuffer;
	ID3D11DepthStencilState*	m_defaultDepthStencil;
	ID3D11DepthStencilState*	m_skyboxDepthStencil;
	ID3D11RasterizerState*		m_defaultRS;
	ID3D11RasterizerState*		m_wireframeRS;
	ID3D11RasterizerState*		m_skyboxRS;
	DX11Framework* m_DX11Framework;
	Text2D*	m_2DText;
	GameObject* gameObject;
	GameObject* cubeObject;
	GameObject* Floor;
	Skybox* m_skybox;
	XMMATRIX m_identity;
	ParticleManager* m_pParticleManager;
	//ParticleGenerator* m_particleGenerator;

	float second;
	int frameCount;
	bool wireframeMode;
	bool showFPS;

	//InitialiseGraphics
	ShaderManager* m_shaderManager;
	TextureManager* m_textureManager;
	materialManager* m_materialManager;
	LightManager* m_lightManger;
	ModelManager* m_pModelManager;
	Camera* m_camera;
	InputManager* m_input;
	GameObject* m_rootGameObject;
	Player* m_player;
	
	void renderSkybox(const XMMATRIX& view, const XMMATRIX& projection);

public:
	Game(InputManager* input, ID3D11Device* device, ID3D11DeviceContext* context);
	~Game();
	HRESULT InitialiseWindow(WNDPROC WndProc, int nCmdShow);
	HRESULT InitialiseD3D();
	HRESULT InitialiseGraphics();

	void setInput(InputManager* input);
	HWND getHWND() { return m_hWnd; };

	void Render(float deltaTime);
	void Update(float deltaTime);
};

#endif