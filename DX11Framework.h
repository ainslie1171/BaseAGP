#ifndef _DX11FRAMEWORK_H_
#define _DX11FRAMEWORK_H_

#include <d3dx11.h>
#include <dxerr.h>
#include "mainMenu.h"
#include "optionsMenu.h"
#include "inputManager.h"
#include "states.h"
#include "game.h"

//TODO Game
//DONE MainMenu
//TODO ShaderManager
//DONE InputManager
//DONE State
//TODO Settings
//DONE DeltaTime

struct Timer
{
private:
	LARGE_INTEGER StartingTime, EndingTime, DeltaTime, Frequency;
	
public:
	void UpdateDeltaTime()
	{
		QueryPerformanceCounter(&EndingTime);
		DeltaTime.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
		DeltaTime.QuadPart *= 1000000;
		DeltaTime.QuadPart /= Frequency.QuadPart;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);
	}

	float calcDeltaTime(LARGE_INTEGER start, LARGE_INTEGER end)
	{
		LARGE_INTEGER dt;
		QueryPerformanceCounter(&end);
		dt.QuadPart = end.QuadPart - start.QuadPart;
		dt.QuadPart *= 1000000;
		dt.QuadPart /= Frequency.QuadPart;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&start);
		return (dt.QuadPart / 1000000.0f);
	}

	float getDeltaTime(void) { return DeltaTime.QuadPart / 1000000.0f; }

	Timer(void)
	{
		StartingTime.QuadPart = 0;
		Frequency.QuadPart = 1;
	}
};

enum RSState
{
	RS_DEFAULT, RS_SKYBOX, RS_WIREFRAME
};

class DX11Framework
{
private:
	//Members
	static DX11Framework* m_pInstance;
	HINSTANCE m_hInst;
	HWND m_hWnd;
	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBufferRTView;
	ID3D11DepthStencilView* m_pZBuffer;
	ID3D11BlendState* m_pAlphaBlendStateEnabled;
	ID3D11BlendState* m_pAlphaBlendStateDisabled;
	ID3D11RasterizerState* m_pDefaultRS;
	ID3D11RasterizerState* m_pSkyboxRS;
	ID3D11RasterizerState* m_pWireframeRS;
	ID3D11DepthStencilState* m_pDefaultDepthStencil;
	ID3D11DepthStencilState* m_pSkyboxDepthStencil;

	float m_clearColour[4];
	Game* m_pGame;
	MainMenu* m_pMenu;
	OptionsMenu* m_pOptions;
	//ShaderManager* m_pShaderManager;
	InputManager* m_pInputManager;
	State m_state;
	//Settings* m_pSettings;
	//DeltaTime
	Timer m_timer;

	//Methods
	DX11Framework(void);
	~DX11Framework(void);
	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitialiseD3D(void);
	HRESULT CreateZBuffer(ID3D11DepthStencilView** p_zBuffer, UINT width, UINT height, UINT zBufferTextureSampleCount);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT ResizeWindow(LPARAM lParam);
	void DestroyFrameworkWindow(void);
	//DeltaTime
public:
	static DX11Framework* getInstance(void);
	static void ReleaseInstance(void);
	int Initialise(HINSTANCE hInstance, int nCmdShow);
	void Update(void);
	void Render(void);

	void setRSState(RSState state);

	//Changed return type to HRESULT
	//might change these parameters to UINTs in the future
	HRESULT SetWindowResolution(UINT width, UINT height);
};
#endif //_DX11FRAMEWORK_H_

/*
Change Log:
-moved the state enum into it's own file

*/