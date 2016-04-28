#include "game.h"
#include "DX11Framework.h"

Game::Game(InputManager* input, ID3D11Device* device, ID3D11DeviceContext* context)
{
	//Input
	m_input = input;
	
	//Texture Manager
	m_textureManager = new TextureManager(device);
	m_textureManager->add("assets/texture.bmp");
	m_textureManager->add("assets/yellow.bmp");

	//Shader Manager
	m_shaderManager = new ShaderManager(device);
	m_shaderManager->add("modelShader.hlsl", 3);
	m_shaderManager->getShader(1)->setCBuffer(device, sizeof(MODEL_CONSTANT_BUFFER));
	m_shaderManager->add("testShader.hlsl", 3);
	m_shaderManager->getShader(2)->setCBuffer(device, sizeof(REFLECT_CONSTANT_BUFFER));
	m_shaderManager->add("particleShader.hlsl", 1);
	m_shaderManager->getShader(3)->setCBuffer(device, sizeof(PARTICLE_CONSTANT_BUFFER));
	/*
	MODEL_CONSTANT_BUFFER cb_values;



	shader->cBuffer = cb_values;
	
	*/

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
	PARTICLE_GENERATOR_DESC pgDesc;
	pgDesc.context = context;
	pgDesc.device = device;
	pgDesc.shaderManager = m_shaderManager;
	pgDesc.targetShader = 3;
	pgDesc.particleSpawnRate = 1.0f;
	m_particleGenerator = new ParticleGenerator(pgDesc);
	m_particleGenerator->create();


	//objects
	m_rootNode = new SceneNode();
	
	MODEL_DESC cubeDesc;
	ZeroMemory(&cubeDesc, sizeof(cubeDesc));
	cubeDesc.context = context;
	cubeDesc.device = device;
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


	m_identity = XMMatrixIdentity();

	//Shader s = m_shaderM
}

Game::~Game()
{
	if (pointySphere) { delete pointySphere; pointySphere = nullptr; }
	if (sphere) { delete sphere; sphere = nullptr; }
	if (cube) { delete cube; cube = nullptr; }
	if (m_rootNode) { delete m_rootNode; m_rootNode = nullptr; }
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
		OutputDebugString(outputString);
		frameCount = 0;
	}
	
	if (m_input->IsKeyPressed(DIK_ESCAPE))
		DestroyWindow(m_hWnd);

	//camera controller
	m_camera->RotateX(m_input->getMouseData().x * m_camera->getRotationSpeed() * deltaTime);
	m_camera->RotateY(m_input->getMouseData().y * m_camera->getRotationSpeed() * -deltaTime);
	if (m_input->IsKeyPressed(DIK_W))
		m_camera->Forward(m_camera->getSpeed()*deltaTime);
	if (m_input->IsKeyPressed(DIK_S))
		m_camera->Forward(-m_camera->getSpeed()*deltaTime);
	if (m_input->IsKeyPressed(DIK_A))
		m_camera->Strafe(-m_camera->getSpeed()*deltaTime);
	if (m_input->IsKeyPressed(DIK_D))
		m_camera->Strafe(m_camera->getSpeed()*deltaTime);
	if (m_input->IsKeyPressed(DIK_Q))
		m_camera->Up(-m_camera->getSpeed()*deltaTime);
	if (m_input->IsKeyPressed(DIK_E))
		m_camera->Up(m_camera->getSpeed()*deltaTime);

	float speed = 3.0f*deltaTime;
	if (m_input->IsKeyPressed(DIK_UP))
		m_player->forward(speed);
	else if (m_input->IsKeyPressed(DIK_DOWN))
		m_player->backward(speed);
	if (m_input->IsKeyPressed(DIK_LEFT))
		m_player->left(speed);
	else if (m_input->IsKeyPressed(DIK_RIGHT))
		m_player->right(speed);

	//if (m_input->getMouseData().leftClick && !m_input->getPrevMouseData().leftClick)
	//{
	//	m_particleGenerator->spawnParticle();
	//}

	if (m_input->IsKeyPressed(DIK_INSERT))
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
	XMMATRIX projection, view;
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 640.0f / 480.0f, 0.01f, 100.0f);
	view = m_camera->getMatrixView();

	//draw skybox
	renderSkybox(view, projection);

	//wireframe?



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
	m_rootNode->draw(desc);

	desc.world = &m_identity;
	//m_particleGenerator->drawP(desc);

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