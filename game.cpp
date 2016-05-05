#include "game.h"
#include "DX11Framework.h"

Game::Game(InputManager* input, ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_DX11Framework = DX11Framework::getInstance();
	DebugLine::getInstance()->Init(device, context);
	//Input
	m_input = input;
	showFPS = false;
	//Texture Manager
	m_textureManager = new TextureManager(device, context);
	m_textureManager->add("assets/texture.bmp");
	m_textureManager->add("assets/yellow.bmp");
	m_textureManager->add("assets/render02.bmp");

	//Shader Manager
	m_shaderManager = new ShaderManager(device, context);
	m_shaderManager->add("modelShader.hlsl", 3);
	m_shaderManager->getShader(1)->setCBuffer(device, sizeof(MODEL_CONSTANT_BUFFER));
	m_shaderManager->add("testShader.hlsl", 3);
	m_shaderManager->getShader(2)->setCBuffer(device, sizeof(REFLECT_CONSTANT_BUFFER));
	m_shaderManager->add("particleShader.hlsl", 1);
	m_shaderManager->getShader(3)->setCBuffer(device, sizeof(PARTICLE_CONSTANT_BUFFER));

	//Light Manager
	m_lightManger = new LightManager();

	//Materials
	m_materialManager = new materialManager();

	//Skybox
	SKYBOX_DESC sbDesc;
	ZeroMemory(&sbDesc, sizeof(sbDesc));
	sbDesc.device = device;
	sbDesc.context = context;
	sbDesc.textureFilepath = "assets/skybox02.dds";
	m_skybox = new Skybox(sbDesc);

	//particle generator
	/*
	PARTICLE_GENERATOR_DESC pgDesc;
	pgDesc.context = context;
	pgDesc.device = device;
	pgDesc.shaderManager = m_shaderManager;
	pgDesc.targetShader = 3;
	pgDesc.particleSpawnRate = 1.0f;
	m_particleGenerator = new ParticleGenerator(pgDesc);
	m_particleGenerator->create();
	*/
	PARTICLE_MANAGER_DESC pmDesc;
	pmDesc.context = context;
	pmDesc.device = device;
	pmDesc.maxNumParticles = 100;
	pmDesc.shaderManager = m_shaderManager;
	m_pParticleManager = new ParticleManager(pmDesc);


	//models
	m_pModelManager = new ModelManager(device, context);
	m_pModelManager->LoadModel("assets/sphere.obj");
	m_pModelManager->LoadModel("assets/cube.obj");
	m_pModelManager->LoadModel("assets/Plane.obj");
	m_pModelManager->LoadModel("assets/Capsule.obj");

	//objects
	GAMEOBJECT_DESC GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GameObjectDesc));

	m_rootGameObject = new GameObject(GameObjectDesc);

	GameObjectDesc.rootGameObject = m_rootGameObject;
	GameObjectDesc.inputManager = input;
	GameObjectDesc.modelManager = m_pModelManager;
	GameObjectDesc.lightManager = m_lightManger;
	GameObjectDesc.shaderManager = m_shaderManager;
	GameObjectDesc.particleManager = m_pParticleManager;
	GameObjectDesc.textureManager = m_textureManager;
	GameObjectDesc.model = m_pModelManager->getModel(3);
	
	GameObjectDesc.targetShader = 2;
	GameObjectDesc.material = m_materialManager->getMaterial(WhitePlastic);
	GameObjectDesc.targetTexture = 3;
	GameObjectDesc.canBeShot = false;
	Floor = new GameObject(GameObjectDesc);
	Floor->setScale(100.0f);
	Floor->setPosition({ 0.0f, -1.0f, 0.0f });
	GameObjectDesc.canBeShot = true;
	GameObjectDesc.targetTexture = 1;
	GameObjectDesc.model = m_pModelManager->getModel(4);
	m_player = new Player(GameObjectDesc, 100);
	m_player->setPosition({ 0.0f, 0.0f, -4.0f });

	m_pickupManager = new PickupManager(m_pParticleManager, m_player);

	GroundEnemy* enemy = new GroundEnemy(GameObjectDesc, 100, m_player, m_pickupManager);
	enemy->setPosition({ 0.0f, -1.0f, 10.0f });


	GameObjectDesc.model = m_pModelManager->getModel(2);
	FlyingEnemy* fenemy = new FlyingEnemy(GameObjectDesc, 100, m_player, m_pickupManager);
	fenemy->setPosition({ 0.0f, 10.0f, 10.0f });


	//GameObjectDesc.model = m_pModelManager->getModel(4);
	//gameObject = new GameObject(GameObjectDesc);
	//gameObject->setPosition({ 0.0f, -1.0f, 10.0f });


	GameObjectDesc.model = m_pModelManager->getModel(1);
	cubeObject = new GameObject(GameObjectDesc);
	cubeObject->setPosition({ 0.0f, 0.7f, -8.0f });
	cubeObject->setScale(0.1f);

	m_rootGameObject->addChild(enemy);
	m_rootGameObject->addChild(fenemy);
	m_rootGameObject->addChild(Floor);
	m_rootGameObject->addChild(m_player);
	//gameObject->addChild(cubeObject);
	
	


	//lights/manager


	Light newLight;
	ZeroMemory(&newLight, sizeof(newLight));
	newLight.Direction = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	newLight.Colour = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	newLight.type = DIRECTIONAL_LIGHT;

	m_lightManger->addLight(newLight);

	//camera
	m_camera = &Camera::getInstance();
	m_camera->setRootNode(m_rootGameObject);

	frameCount = 0;
	second = 0.0f;


	m_identity = XMMatrixIdentity();

	wireframeMode = false;
	//Shader s = m_shaderM
}

Game::~Game()
{
	//if (pointySphere) { delete pointySphere; pointySphere = nullptr; }
	//if (sphere) { delete sphere; sphere = nullptr; }
	//if (cube) { delete cube; cube = nullptr; }
	//if (m_rootNode) { delete m_rootNode; m_rootNode = nullptr; }
	if (m_rootGameObject) { delete m_rootGameObject; m_rootGameObject = nullptr; }
	if (m_skybox){ delete m_skybox; m_skybox = nullptr; }
	if (m_materialManager){ delete m_materialManager; m_materialManager = nullptr; }
	if (m_lightManger){ delete m_lightManger; m_lightManger = nullptr; }
	if (m_shaderManager){ delete m_shaderManager; m_shaderManager = nullptr; }
	if (m_textureManager){ delete m_textureManager; m_textureManager = nullptr; }
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
		if (showFPS) OutputDebugString(outputString);
		frameCount = 0;
	}
	
	if (m_input->IsKeyPressed(DIK_ESCAPE))
		DestroyWindow(m_hWnd);


	if (m_input->IsKeyPressedOnce(DIK_DELETE))
	{
		gameObject->Remove();
	}

	if (m_input->IsKeyPressedOnce(DIK_SCROLL))
	{
		wireframeMode = !wireframeMode;
	}

	if (m_input->IsKeyPressed(DIK_LCONTROL) && m_input->IsKeyPressedOnce(DIK_F))
		showFPS = !showFPS;

	//m_player->Update(deltaTime);
	m_rootGameObject->Update(deltaTime);
	m_rootGameObject->cleanUp();
	m_pParticleManager->Update(deltaTime);
	m_pickupManager->Update(deltaTime);
}

void Game::Render(float deltaTime)

{
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
		m_DX11Framework->setRSState(RS_WIREFRAME);
		m_rootGameObject->drawBoundingSphere(descWS);
		m_DX11Framework->setRSState(RS_DEFAULT);
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
	m_rootGameObject->Render(desc);

	desc.world = &m_identity;
	m_pParticleManager->Render(desc);
	DebugLine::getInstance()->Render(m_identity*view*projection);
}

void Game::renderSkybox(const XMMATRIX& view, const XMMATRIX& projection)
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix *= XMMatrixScaling(3.0f, 3.0f, 3.0f);
	worldMatrix *= XMMatrixTranslationFromVector(m_camera->getPosition().getXMVector());

	DX11Framework::getInstance()->setRSState(RS_SKYBOX);

	m_skybox->Draw(worldMatrix, view, projection);

	DX11Framework::getInstance()->setRSState(RS_DEFAULT);
}