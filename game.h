#ifndef _GAME_H_
#define _GAME_H_

#include "input.h"
#include <d3dx11.h>
#include "camera.h"
#include <vector>
#include "maths.h"
#include "shaderManager.h"
#include "particleGenerator.h"
#include "plane.h"

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

	ParticleGenerator* m_particleGenerator;
	p_Plane* m_pPlane;

	struct wall
	{
		Vector3 p;
		Vector3 r;
	};

	std::vector<wall> m_walls;

	float second;
	int frameCount;
	float pCounter;


	//InitialiseGraphics
	ShaderManager* m_shaderManager;

	Camera* m_camera;
	Input* m_input;


public:
	Game(HINSTANCE hInstance);
	~Game();
	HRESULT InitialiseWindow(WNDPROC WndProc, int nCmdShow);
	HRESULT InitialiseD3D();
	HRESULT InitialiseGraphics();

	void setInput(Input* input);
	HWND getHWND() { return m_hWnd; };

	void Render(void);
	void Update(float deltaTime);
};

#endif