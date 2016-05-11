#include "game.h"



Game::Game(HINSTANCE hInstance)
{
	m_hInst = hInstance;
	strncpy_s(m_TutorialName, "Project\0", 100);
	pCounter = 0.0f;
}

Game::~Game()
{
	if (m_shaderManager){ delete m_shaderManager; m_shaderManager = nullptr; }
	if (m_pZBuffer) m_pZBuffer->Release();//06-01b
	if (m_pBackBufferRTView) m_pBackBufferRTView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pD3DDevice) m_pD3DDevice->Release();
}

HRESULT Game::InitialiseWindow(WNDPROC WndProc, int nCmdShow)
{
	// Give your app window your own name
	char Name[100] = "Ainslie Cleverdon\0";
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
	m_hWnd = CreateWindow(Name, m_TutorialName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, m_hInst, NULL);
	if (!m_hWnd)
		return E_FAIL;

	ShowWindow(m_hWnd, nCmdShow);

	return S_OK;
}

HRESULT Game::InitialiseD3D()
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

	//Create Z buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
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

	m_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &m_pZBuffer);
	pZBufferTexture->Release();

	// Set the render target view
	m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBufferRTView, m_pZBuffer);

	// Set the viewporta
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_pImmediateContext->RSSetViewports(1, &viewport);

	return S_OK;
}

HRESULT Game::InitialiseGraphics()
{
	float offset = 0.3f;

	m_pPlane = new p_Plane(m_pD3DDevice, m_pImmediateContext, { 0.0f, 0.0f, (5.0f + offset) }, ZeroVector3, 5.0f + offset);

	offset += 5.0f;

	wall w;
	//back
	w.p = { 0.0f, 0.0f, offset};
	w.r = ZeroVector3;
	m_walls.push_back(w);
	//front
	w.p = { 0.0f, 0.0f, -offset};
	w.r = {0.0f, 180.0f, 0.0f};
	m_walls.push_back(w);
	//left
	w.p = { -offset, 0.0f, 0.0f };
	w.r = { 0.0f, -90.0f, 0.0f };
	m_walls.push_back(w);
	//right
	w.p = { offset, 0.0f, 0.0f };
	w.r = { 0.0f, 90.0f, 0.0f };
	m_walls.push_back(w);
	//floor
	w.p = { 0.0f, -offset, 0.0f };
	w.r = {90.0f, 0.0f, 0.0f};
	m_walls.push_back(w);


	m_shaderManager = new ShaderManager(m_pD3DDevice);
	m_shaderManager->add("particleShader.hlsl", 1);


	//particle generator
	m_particleGenerator = new ParticleGenerator(m_pD3DDevice, m_pImmediateContext);
	//m_particleGenerator->create();
	
	//camera
	m_camera = &Camera::getInstance();
	m_camera->setPosition({ -12.0f, 5.0f, -12.0f, 0.0f });
	m_camera->RotateX(45.0f *(XM_PI / 180.0f));
	m_camera->RotateY(20.0f * (XM_PI / 180.0f));
	
	frameCount = 0;
	second = 0.0f;

	return S_OK;
}

void Game::setInput(Input* input)
{
	m_input = input;
}

void Game::Update(float deltaTime)
{
	frameCount++;
	second += deltaTime;
	if ( second >= 1.0f)
	{
		second = 0;
		char outputString[50];
		sprintf_s(outputString, "FPS: %d\n\n", frameCount);
		OutputDebugString(outputString);
		frameCount = 0;
	}
	
	if (m_input->isKeyPressed(DIK_ESCAPE))
		DestroyWindow(m_hWnd);

	//camera controller
	m_camera->RotateX(m_input->getMouseData().x * m_camera->getRotationSpeed() * deltaTime * XM_PI / 180.0f);
		m_camera->RotateY(m_input->getMouseData().y * m_camera->getRotationSpeed() * deltaTime * XM_PI / 180.0f);
	if (m_input->isKeyPressed(DIK_W))
		m_camera->Forward(m_camera->getSpeed()*deltaTime);
	if (m_input->isKeyPressed(DIK_S))
		m_camera->Forward(-m_camera->getSpeed()*deltaTime);
	if (m_input->isKeyPressed(DIK_A))
		m_camera->Strafe(-m_camera->getSpeed()*deltaTime);
	if (m_input->isKeyPressed(DIK_D))
		m_camera->Strafe(m_camera->getSpeed()*deltaTime);
	if (m_input->isKeyPressed(DIK_Q))
		m_camera->Up(-m_camera->getSpeed()*deltaTime);
	if (m_input->isKeyPressed(DIK_E))
		m_camera->Up(m_camera->getSpeed()*deltaTime);


	if (m_input->getMouseData().leftClick && !m_input->getPrevMouseData().leftClick)
	{
		m_particleGenerator->spawnParticle();
	}

	pCounter += deltaTime;
	if (pCounter >= 1.0f / 60.0f)
	{
		m_particleGenerator->update(pCounter);

		pCounter = 0.0f;
	}
}

void Game::Render(void)
{
	// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 0.0f, 0.0f, 0.0f, 0.01f };
	m_pImmediateContext->ClearRenderTargetView(m_pBackBufferRTView, rgba_clear_colour);
	//Clear the Z buffer
	m_pImmediateContext->ClearDepthStencilView(m_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX world, projection, view;
	world = XMMatrixIdentity();
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 640.0f / 480.0f, 0.01f, 100.0f);
	view = m_camera->getMatrixView();

	RENDER_DESC2 desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.projection = &projection;
	desc.view = &view;
	desc.world = &world;

	XMVECTOR c = m_camera->getPosition().getXMVector();
	desc.camera = &c;
	
	m_pImmediateContext->VSSetShader(m_shaderManager->getShader(1)->VShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_shaderManager->getShader(1)->PShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_shaderManager->getShader(1)->InputLayout);

	m_particleGenerator->drawP(desc);
	

	for (wall w : m_walls)
	{
		m_pPlane->setPos(w.p);
		m_pPlane->setRot(w.r);
		m_pPlane->draw((view * projection));
	}

	// Display what has just been rendered
	m_pSwapChain->Present(0, 0);
}