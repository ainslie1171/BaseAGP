#include "DX11Framework.h"

DX11Framework* DX11Framework::m_pInstance = new DX11Framework;

DX11Framework::DX11Framework(void)
{
	for (float& i : m_clearColour)
	{
		i = 0.0f;
	}

	m_state = STATE_MENU;
}

DX11Framework::~DX11Framework(void)
{
	if (m_pZBuffer) m_pZBuffer->Release();
	if (m_pBackBufferRTView) m_pBackBufferRTView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pD3DDevice) m_pD3DDevice->Release();
	if (m_pInputManager) delete m_pInputManager; m_pInputManager = nullptr;
	DestroyFrameworkWindow();
}

HRESULT DX11Framework::InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
	m_hInst = hInstance;

	// Give your app window your own name
	char Name[100] = "Ainslie Cleverdon\0";
	char AppName[100] = "AGP Project\0";
#ifdef _SHOW_CONSOLE_
	AllocConsole();
#endif

	// Register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = m_hInst;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//   wcex.hbrBackground = (HBRUSH )( COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = Name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	// Create window
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(Name, AppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, m_hInst, NULL);
	if (!m_hWnd)
		return E_FAIL;

	ShowWindow(m_hWnd, nCmdShow);

	return S_OK;
}

HRESULT DX11Framework::InitialiseD3D(void)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // comment out this line if you need to test D3D 11.0 functionality on hardware that doesn't support it
		D3D_DRIVER_TYPE_WARP, // comment this out also to use reference device
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_pSwapChain,
			&m_pD3DDevice, &m_featureLevel, &m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	// Get pointer to back buffer texture
	ID3D11Texture2D *pBackBufferTexture;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBufferTexture);

	if (FAILED(hr)) return hr;

	// Use the back buffer texture pointer to create the render target view
	hr = m_pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL,
		&m_pBackBufferRTView);
	pBackBufferTexture->Release();

	if (FAILED(hr)) return hr;

	hr = CreateZBuffer(&m_pZBuffer, width, height, sd.SampleDesc.Count);

	if (FAILED(hr)) return hr;

	// Set the viewporta
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_pImmediateContext->RSSetViewports(1, &viewport);

	//TODO 2D Text
	//m_2DText = new Text2D("assets/font1.bmp", m_pD3DDevice, m_pImmediateContext);


	//TODO Rasterizer States?
	
	D3D11_RASTERIZER_DESC defaultDesc;
	ZeroMemory(&defaultDesc, sizeof(defaultDesc));
	defaultDesc.FillMode = D3D11_FILL_SOLID;
	defaultDesc.CullMode = D3D11_CULL_BACK;
	defaultDesc.DepthClipEnable = true;

	hr = m_pD3DDevice->CreateRasterizerState(&defaultDesc, &m_pDefaultRS);

	if (FAILED(hr)) return hr;

	D3D11_RASTERIZER_DESC wireframeRSdesc;
	ZeroMemory(&wireframeRSdesc, sizeof(wireframeRSdesc));
	wireframeRSdesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeRSdesc.CullMode = D3D11_CULL_NONE;
	wireframeRSdesc.DepthClipEnable = true;

	hr = m_pD3DDevice->CreateRasterizerState(&wireframeRSdesc, &m_pWireframeRS);

	if (FAILED(hr)) return hr;

	D3D11_RASTERIZER_DESC skyboxRSdesc;
	ZeroMemory(&skyboxRSdesc, sizeof(skyboxRSdesc));
	skyboxRSdesc.FillMode = D3D11_FILL_SOLID;
	skyboxRSdesc.CullMode = D3D11_CULL_FRONT;
	skyboxRSdesc.DepthClipEnable = true;

	hr = m_pD3DDevice->CreateRasterizerState(&skyboxRSdesc, &m_pSkyboxRS);

	if (FAILED(hr)) return hr;
	

	//TODO DepthStencil States?
	
	D3D11_DEPTH_STENCIL_DESC defaultDepthStencilDesc;
	ZeroMemory(&defaultDepthStencilDesc, sizeof(defaultDepthStencilDesc));
	defaultDepthStencilDesc.DepthEnable = true;
	defaultDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	defaultDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = m_pD3DDevice->CreateDepthStencilState(&defaultDepthStencilDesc, &m_pDefaultDepthStencil);

	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_DESC skyboxDepthStencilDesc;
	ZeroMemory(&skyboxDepthStencilDesc, sizeof(skyboxDepthStencilDesc));
	skyboxDepthStencilDesc.DepthEnable = true;
	skyboxDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	skyboxDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = m_pD3DDevice->CreateDepthStencilState(&skyboxDepthStencilDesc, &m_pSkyboxDepthStencil);

	if (FAILED(hr)) return hr;
	

	return S_OK;
}

HRESULT DX11Framework::CreateZBuffer(ID3D11DepthStencilView** p_zBuffer, UINT width, UINT height, UINT zBufferTextureSampleCount)
{
	HRESULT hr;
	//Create Z buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = zBufferTextureSampleCount;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *pZBufferTexture;
	hr = m_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;

	//create the Z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	m_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, p_zBuffer);
	pZBufferTexture->Release();

	// Set the render target view
	m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBufferRTView, *p_zBuffer);

	return S_OK;
}

LRESULT CALLBACK DX11Framework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		//m_pInstance->ResizeWindow(lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT DX11Framework::ResizeWindow(LPARAM lParam)
{
	m_pImmediateContext->OMSetRenderTargets(0, 0, 0);
	m_pBackBufferRTView->Release();
	HRESULT hr;
	hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr)) return hr;

	ID3D11Texture2D* pBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	if (FAILED(hr)) return hr;

	hr = m_pD3DDevice->CreateRenderTargetView(pBuffer, NULL, &m_pBackBufferRTView);
	if (FAILED(hr)) return hr;

	pBuffer->Release();

	m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBufferRTView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = LOWORD(lParam);
	vp.Height = HIWORD(lParam);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);
	return S_OK;
}

void DX11Framework::DestroyFrameworkWindow(void)
{
	DestroyWindow(m_hWnd);
}

DX11Framework* DX11Framework::getInstance(void)
{
	if (!m_pInstance) m_pInstance = new DX11Framework;
	return m_pInstance;
}

void DX11Framework::ReleaseInstance(void)
{
	if (m_pInstance) delete m_pInstance;
}

int DX11Framework::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	if (FAILED(InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to create Device");
		return 0;
	}

	m_pInputManager = new InputManager(m_hInst, m_hWnd);
	m_pMenu = new MainMenu(m_pD3DDevice, m_pImmediateContext, m_pInputManager);
	m_pOptions = new OptionsMenu(m_pD3DDevice, m_pImmediateContext, m_pInputManager);
	m_pGame = new Game(m_pInputManager, m_pD3DDevice, m_pImmediateContext);
	

	return 1;
}

void DX11Framework::Update(void)
{
	m_timer.UpdateDeltaTime();
	m_pInputManager->ReadInputStates();

	switch (m_state)
	{
	case STATE_MENU:
		m_pMenu->Update(m_state);
		break;
	case STATE_OPTIONS:
		m_pOptions->Update(m_state);
		break;
	case STATE_GAME:
		m_pGame->Update(m_timer.getDeltaTime());
		break;
	case STATE_QUIT:
		DX11Framework::~DX11Framework();
		return;
	default:
		break;
	}

	if (m_pInputManager->IsKeyPressed(DIK_ESCAPE))
		DX11Framework::~DX11Framework();
}

void DX11Framework::Render(void)
{
	m_pImmediateContext->ClearRenderTargetView(m_pBackBufferRTView, m_clearColour);
	m_pImmediateContext->ClearDepthStencilView(m_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	switch (m_state)
	{
	case STATE_MENU:
		m_pMenu->Render();
		break;
	case STATE_OPTIONS:
		m_pOptions->Render();
		break;
	case STATE_GAME:
		m_pGame->Render(m_timer.getDeltaTime());
		break;
	default:
		break;
	}
	m_pSwapChain->Present(0, 0);
}


HRESULT DX11Framework::SetWindowResolution(UINT width, UINT height)
{
	m_pImmediateContext->OMSetRenderTargets(0, 0, 0);
	m_pBackBufferRTView->Release();
	HRESULT hr;
	hr = m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr)) return hr;

	ID3D11Texture2D* pBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	if (FAILED(hr)) return hr;

	hr = m_pD3DDevice->CreateRenderTargetView(pBuffer, NULL, &m_pBackBufferRTView);
	if (FAILED(hr)) return hr;

	pBuffer->Release();

	m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBufferRTView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);
	return S_OK;
}

void DX11Framework::setRSState(RSState state)
{
	switch (state)
	{
	case RS_DEFAULT:
		m_pImmediateContext->RSSetState(m_pDefaultRS);
		m_pImmediateContext->OMSetDepthStencilState(m_pDefaultDepthStencil, 0);
		break;
	case RS_SKYBOX:
		m_pImmediateContext->RSSetState(m_pSkyboxRS);
		m_pImmediateContext->OMSetDepthStencilState(m_pSkyboxDepthStencil, 0);
		break;
	case RS_WIREFRAME:
		m_pImmediateContext->RSSetState(m_pWireframeRS);
		break;
	}
}