#include "materialManager.h"

materialManager::materialManager()
{
	//create materials
	Material* test = new Material();
	test->Ambient = XMVectorSet(0.25f, 0.25f, 0.25f, 1.0f);// (1.0f, 1.0f, 1.0f, 1.0f);
	test->Diffuse = XMVectorSet(0.4f, 0.4f, 0.4f, 1.0f);
	test->Emissive = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	test->Specular = XMVectorSet(0.774597f, 0.774597f, 0.774597f, 1.0f);
	test->specularPower = 76.8f;
	m_Materials.push_back(test);

#define WhitePlastic 0;
	Material* whitePlastic = new Material();
	whitePlastic->Ambient = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	whitePlastic->Diffuse = XMVectorSet(0.55f, 0.55f, 0.55f, 1.0f);
	whitePlastic->Emissive = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	whitePlastic->Specular = XMVectorSet(0.7f, 0.7f, 0.7f, 1.0f);
	whitePlastic->specularPower = 32;
	m_Materials.push_back(whitePlastic);

#define YellowPlastic 1;
	Material* yellowPlastic = new Material();
	yellowPlastic->Ambient = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
	yellowPlastic->Diffuse = XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);
	yellowPlastic->Emissive = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	yellowPlastic->Specular = XMVectorSet(0.6f, 0.6f, 0.5f, 1.0f);
	yellowPlastic->specularPower = 32;
	m_Materials.push_back(yellowPlastic);

#define RedPlastic 2;
	Material* redPlastic = new Material();
	redPlastic->Ambient = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	redPlastic->Diffuse = XMVectorSet(0.61424f, 0.04136f, 0.04136f, 1.0f);
	redPlastic->Emissive = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	redPlastic->Specular = XMVectorSet(0.727811f, 0.626959f, 0.626959f, 1.0f);
	redPlastic->specularPower = 76.8f;
	m_Materials.push_back(redPlastic);

	Material* gold = new Material();
	gold->Ambient = XMVectorSet(0.24725f, 0.1995f, 0.0745f, 1.0f);
	gold->Diffuse = XMVectorSet(0.75164f, 0.60648f, 0.22648f, 1.0f);
	gold->Emissive = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	gold->Specular = XMVectorSet(0.628281f, 0.555802f, 0.366065f, 1.0f);
	gold->specularPower = 51.2f;
	m_Materials.push_back(gold);
}

materialManager::~materialManager()
{
	m_Materials.clear();
}