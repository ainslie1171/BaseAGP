#include "game.h"

Game::Game(HINSTANCE hInstance)
{
	m_hInst = hInstance;
	strncpy_s(m_TutorialName, "SWD304 - Tutorial 15 Exercise 01\0", 100);
}

Game::~Game()
{
	
	if (m_2DText) { delete m_2DText; m_2DText = nullptr; }

	if (pointySphere) { delete pointySphere; pointySphere = nullptr; }
	if (sphere) { delete sphere; sphere = nullptr; }
	if (cube) { delete cube; cube = nullptr; }
	if (m_rootNode) { delete m_rootNode; m_rootNode = nullptr; }
	if (m_skybox){ delete m_skybox; m_skybox = nullptr; }
	if (m_materialManager){ delete m_materialManager; m_materialManager = nullptr; }
	if (m_lightManger){ delete m_lightManger; m_lightManger = nullptr; }
	if (m_shaderManager){ delete m_shaderManager; m_shaderManager = nullptr; }
	if (m_textureManager){ delete m_textureManager; m_textureManager = nullptr; }

	if (m_pZBuffer) m_pZBuffer->Release();//06-01b

	if (m_wireframeRS) m_wireframeRS->Release();
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

	m_2DText = new Text2D("assets/font1.bmp", m_pD3DDevice, m_pImmediateContext);

	D3D11_RASTERIZER_DESC defaultDesc;
	ZeroMemory(&defaultDesc, sizeof(defaultDesc));
	defaultDesc.FillMode = D3D11_FILL_SOLID;
	defaultDesc.CullMode = D3D11_CULL_BACK;
	defaultDesc.DepthClipEnable = true;

	hr = m_pD3DDevice->CreateRasterizerState(&defaultDesc, &m_defaultRS);

	if (FAILED(hr)) return hr;

	D3D11_RASTERIZER_DESC wireframeRSdesc;
	ZeroMemory(&wireframeRSdesc, sizeof(wireframeRSdesc));
	wireframeRSdesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeRSdesc.CullMode = D3D11_CULL_NONE;
	wireframeRSdesc.DepthClipEnable = true;

	hr = m_pD3DDevice->CreateRasterizerState(&wireframeRSdesc, &m_wireframeRS);

	if (FAILED(hr)) return hr;

	D3D11_RASTERIZER_DESC skyboxRSdesc;
	ZeroMemory(&skyboxRSdesc, sizeof(skyboxRSdesc));
	skyboxRSdesc.FillMode = D3D11_FILL_SOLID;
	skyboxRSdesc.CullMode = D3D11_CULL_FRONT;
	skyboxRSdesc.DepthClipEnable = true;

	hr = m_pD3DDevice->CreateRasterizerState(&skyboxRSdesc, &m_skyboxRS);

	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_DESC defaultDepthStencilDesc;
	ZeroMemory(&defaultDepthStencilDesc, sizeof(defaultDepthStencilDesc));
	defaultDepthStencilDesc.DepthEnable = true;
	defaultDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	defaultDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = m_pD3DDevice->CreateDepthStencilState(&defaultDepthStencilDesc, &m_defaultDepthStencil);

	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_DESC skyboxDepthStencilDesc;
	ZeroMemory(&skyboxDepthStencilDesc, sizeof(skyboxDepthStencilDesc));
	skyboxDepthStencilDesc.DepthEnable = true;
	skyboxDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	skyboxDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = m_pD3DDevice->CreateDepthStencilState(&skyboxDepthStencilDesc, &m_skyboxDepthStencil);

	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT Game::InitialiseGraphics()
{
	//HRESULT hr = S_OK;

	//Shader* s = new Shader(m_pD3DDevice, "modelShader.hlsl");
	//m_shaders.push_back(s);

	m_textureManager = new TextureManager(m_pD3DDevice);
	m_textureManager->add("assets/texture.bmp");
	m_textureManager->add("assets/yellow.bmp");
	//m_textureManager->add("assets.spherelight.png");

	m_shaderManager = new ShaderManager(m_pD3DDevice);

	m_shaderManager->add("modelShader.hlsl", 3);
	m_shaderManager->add("testShader.hlsl", 3);
	m_shaderManager->add("particleShader.hlsl", 1);

	m_lightManger = new LightManager();

	//Materials
	m_materialManager = new materialManager();

	SKYBOX_DESC sbDesc;
	ZeroMemory(&sbDesc, sizeof(sbDesc));
	sbDesc.device = m_pD3DDevice;
	sbDesc.context = m_pImmediateContext;
	sbDesc.textureFilepath = "assets/skybox02.dds";
	m_skybox = new Skybox(sbDesc);

	//particle generator
	PARTICLE_GENERATOR_DESC pgDesc;
	pgDesc.context = m_pImmediateContext;
	pgDesc.device = m_pD3DDevice;
	pgDesc.shaderManager = m_shaderManager;
	pgDesc.targetShader = 3;
	pgDesc.particleSpawnRate = 1.5f;
	m_particleGenerator = new ParticleGenerator(pgDesc);

	//objects
	m_rootNode = new SceneNode();
	
	MODEL_DESC cubeDesc;
	ZeroMemory(&cubeDesc, sizeof(cubeDesc));
	cubeDesc.context = m_pImmediateContext;
	cubeDesc.device = m_pD3DDevice;
	cubeDesc.lightManager = m_lightManger;
	cubeDesc.shaderManager = m_shaderManager;
	cubeDesc.textureManager = m_textureManager;
	cubeDesc.filePath = "assets/cube.obj";
	cubeDesc.targetTexture = 1;
	cubeDesc.targetShader = 2;
	cubeDesc.material = m_materialManager->getMaterial(WhitePlastic);

	cube = new Model(cubeDesc);
	//cube->LoadObjModel("assets/cube.obj");
	//cube->AddTexture("assets/yellow.bmp");
	//cube->setMaterial(m_materialManager->getMaterial(WhitePlastic));
	//cube->toggleUnlit(true);
	//m_models.push_back(cube);

	cubeDesc.filePath = "assets/Sphere.obj";
	//cubeDesc.targetTexture = 2;
	cubeDesc.material = m_materialManager->getMaterial(Gold);
	sphere = new Model(cubeDesc);
	//sphere->LoadObjModel("assets/Sphere.obj");
	//sphere->AddTexture("assets/texture.bmp");
	//sphere->setMaterial(m_materialManager->getMaterial(Gold));
	sphere->toggleUnlit(true);
	//m_models.push_back(sphere);
	cubeDesc.targetTexture = 0;
	cubeDesc.material = m_materialManager->getMaterial(WhitePlastic);
	pointySphere = new Model(cubeDesc);
	//pointySphere->LoadObjModel("assets/Sphere.obj");
	//pointySphere->AddTexture("assets/texture.bmp");
	//pointySphere->setMaterial(m_materialManager->getMaterial(test));
	//pointySphere->setPosition({ 15.0f, 0.0f, 0.0f, 0.0f });
	//pointySphere->setXPos(15.0f);
	//pointySphere->toggleUnlit(true);
	//m_models.push_back(pointySphere);
	//2.93333411
	//TODO work out where these are being deleted/make sure that they are
	//SceneNode* cubeNode = new SceneNode(m_rootNode, cube);
	//cubeNode->setPosition(XMVectorSet(-4.0f, 0.0f, 5.0f, 0.0f));
	//cubeNode->setScale(XMVectorSet(0.5, 0.5, 0.5, 0.0));
	//cubeNode->addBoundingSphereModel(sphere);
	SceneNode* cubeNode2 = new SceneNode(m_rootNode, pointySphere);
	cubeNode2->setScale(XMVectorSet(0.1f, 0.1f, 0.1f, 0.0));
	cubeNode2->setPosition(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	cubeNode2->addBoundingSphereModel(sphere);
	//m_rootNode->addNode(cubeNode);
	m_rootNode->addNode(cubeNode2);
	m_player = cubeNode2;

	//lights/manager
	
	
	Light newLight;
	ZeroMemory(&newLight, sizeof(newLight));
	newLight.Direction = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	newLight.Colour = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	newLight.type = DIRECTIONAL_LIGHT;

	m_lightManger->addLight(newLight);
	
	//camera
	m_camera = &Camera::getInstance();
	m_camera->setRootNode(m_rootNode);
	m_camera->setPosition({ 0.0f, 0.0f, -5.0f, 0.0f });
	
	frameCount = 0;
	second = 0.0f;

	wireframeMode = false;

	m_identity = XMMatrixIdentity();

	//Shader s = m_shaderM

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
		XMVECTOR c = m_camera->getVectorView();
		second = 0;
		char outputString[50];
		sprintf_s(outputString, "FPS: %d\n\n", frameCount);
		OutputDebugString(outputString);
		frameCount = 0;
	}
	
	if (m_input->isKeyPressed(DIK_ESCAPE))
		DestroyWindow(m_hWnd);

	//camera controller
	m_camera->RotateX(m_input->getMouseData().x * m_camera->getRotationSpeed() * deltaTime);
	m_camera->RotateY(m_input->getMouseData().y * m_camera->getRotationSpeed() * -deltaTime);
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

	float speed = 3.0f*deltaTime;
	if (m_input->isKeyPressed(DIK_UP))
		m_player->forward(speed);
	else if (m_input->isKeyPressed(DIK_DOWN))
		m_player->backward(speed);
	if (m_input->isKeyPressed(DIK_LEFT))
		m_player->left(speed);
	else if (m_input->isKeyPressed(DIK_RIGHT))
		m_player->right(speed);

	if (m_input->isKeyPressed(DIK_INSERT))
	{
		wireframeMode = !wireframeMode;
		if (wireframeMode)
		{
			//for (Model* m : m_models)
			//{
			//	m->toggleUnlit(wireframeMode);
			//	m->setMaterial(m_materialManager->getMaterial(RedPlastic));
			//}
			//sphere->setMaterial(m_materialManager->getMaterial(YellowPlastic));
		}
		else
		{
			//for (Model* m : m_models)
			//{
			//	m->toggleUnlit(wireframeMode);
			//	m->setMaterial(m_materialManager->getMaterial(WhitePlastic));
			//}
		}
	}
}

void Game::Render(float deltaTime)
{
	// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 0.1f, 0.1f, 0.8f, 0.01f };
	m_pImmediateContext->ClearRenderTargetView(m_pBackBufferRTView, rgba_clear_colour);

	//Clear the Z buffer
	m_pImmediateContext->ClearDepthStencilView(m_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX projection, view;
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 640.0f / 480.0f, 0.01f, 100.0f);
	view = m_camera->getMatrixView();

	//draw skybox
	renderSkybox(view, projection);

	//wireframe?
	if (wireframeMode)
	{
		RENDER_DESC descWS;
		ZeroMemory(&descWS, sizeof(descWS));
		descWS.projection = &projection;
		descWS.view = &view;
		descWS.world = &m_identity;
		descWS.targetShader = 0;
		descWS.targetTexture = 0;
		m_pImmediateContext->RSSetState(m_wireframeRS);
		m_rootNode->drawBoundingSphere(descWS);
	}
	else
	{
		m_pImmediateContext->RSSetState(m_defaultRS);
	}


	RENDER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.projection = &projection;
	desc.view = &view;
	desc.world = &m_identity;
	desc.targetShader = 0;
	desc.targetTexture = 0;
	desc.skyboxTexture = m_skybox->getTexture();
	XMVECTOR c = m_camera->getPosition().getXMVector();
	desc.camera = &c;
	desc.deltaTime = deltaTime;
	XMVECTOR vv = m_camera->getVectorView();
	desc.viewVector = &vv;
	//render objects
	//m_rootNode->draw(desc);

	desc.world = &m_identity;
	m_particleGenerator->draw(desc);

	// Display what has just been rendered
	m_pSwapChain->Present(0, 0);
}

void Game::renderSkybox(const XMMATRIX& view, const XMMATRIX& projection)
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix *= XMMatrixScaling(3.0f, 3.0f, 3.0f);
	worldMatrix *= XMMatrixTranslationFromVector(m_camera->getPosition().getXMVector());

	m_pImmediateContext->RSSetState(m_skyboxRS);
	m_pImmediateContext->OMSetDepthStencilState(m_skyboxDepthStencil, 0);

	m_skybox->Draw(worldMatrix, view, projection);

	m_pImmediateContext->RSSetState(m_defaultRS);
	m_pImmediateContext->OMSetDepthStencilState(m_defaultDepthStencil, 0);
}