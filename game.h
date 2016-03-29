#ifndef _GAME_H_
#define _GAME_H_

#include "input.h"
#include <d3dx11.h>
#include "text2D.h"
#include "camera.h"
#include "materialManager.h"
#include "lightManager.h"
#include "sceneNode.h"
#include "maths.h"
#include "skybox.h"
#include "reflectModel.h"
#include "testModel.h"
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
	Text2D*	m_2DText;
	TestModel* cube;
	Model* sphere;
	TestModel* pointySphere;
	vector<Model*> m_models;
	Skybox* m_skybox;

	float second;
	int frameCount;
	bool wireframeMode;

	//InitialiseGraphics
	materialManager* m_materialManager;
	LightManager* m_lightManger;
	Camera* m_camera;
	Input* m_input;
	SceneNode* m_rootNode;
	SceneNode* m_player;
	
	void renderSkybox(const XMMATRIX& view, const XMMATRIX& projection);

public:
	Game(HINSTANCE hInstance);
	~Game();
	HRESULT InitialiseWindow(WNDPROC WndProc, int nCmdShow);
	HRESULT InitialiseD3D();
	HRESULT InitialiseGraphics();

	void setInput(Input* input);
	HWND getHWND() { return m_hWnd; };

	void Render();
	void Update(float deltaTime);
};

#endif