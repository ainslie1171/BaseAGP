#include "lightManager.h"

using namespace std;

LightManager::LightManager()
{

}

LightManager::~LightManager()
{
	//TODO: fix light manager destructor
	m_lights.clear();
}

void LightManager::addLight(const Light& light)
{
	if (m_lights.size() < MAX_LIGHTS)
	{
		Light* newLight = new Light;
		*newLight = light;
		m_lights.push_back(newLight);
	}
}

void LightManager::renderLights(const XMMATRIX& modelMatrix, MODEL_CONSTANT_BUFFER* cb0)
{
	XMMATRIX transpose = XMMatrixTranspose(modelMatrix);
	XMVECTOR determinant;
	XMMATRIX inverse = XMMatrixInverse(&determinant, modelMatrix);
	
	cb0->ambient_light_colour = m_ambientLight;
	cb0->lightCount = m_lights.size();
	cb0->InvTransWorldMatrix = inverse;

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		switch (m_lights[i]->type)
		{
		case DIRECTIONAL_LIGHT:
			cb0->lights[i].Direction = XMVector3Transform(m_lights[i]->Direction, transpose);
			cb0->lights[i].Direction = XMVector3Normalize(cb0->lights[i].Direction);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			break;
		case POINT_LIGHT:
			cb0->lights[i].Position = XMVector3Transform(m_lights[i]->Position, inverse);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			cb0->lights[i].constAttenuation = m_lights[i]->constAttenuation;
			cb0->lights[i].linAttenuation = m_lights[i]->linAttenuation;
			cb0->lights[i].quadAttenuation = m_lights[i]->quadAttenuation;
			break;
		case SPOT_LIGHT:
			cb0->lights[i].Position = XMVector3Transform(m_lights[i]->Position, inverse);
			cb0->lights[i].Direction = XMVector3Transform(m_lights[i]->Direction, transpose);
			cb0->lights[i].Direction = XMVector3Normalize(cb0->lights[i].Direction);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			cb0->lights[i].constAttenuation = m_lights[i]->constAttenuation;
			cb0->lights[i].linAttenuation = m_lights[i]->linAttenuation;
			cb0->lights[i].quadAttenuation = m_lights[i]->quadAttenuation;
			cb0->lights[i].Angle = m_lights[i]->Angle;
			break;
		}
	}
}

void LightManager::renderLights(const XMMATRIX& modelMatrix, REFLECT_CONSTANT_BUFFER* cb0)
{
	XMMATRIX transpose = XMMatrixTranspose(modelMatrix);
	XMVECTOR determinant;
	XMMATRIX inverse = XMMatrixInverse(&determinant, modelMatrix);

	cb0->ambient_light_colour = m_ambientLight;
	cb0->lightCount = m_lights.size();
	cb0->InvTransWorldMatrix = inverse;

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		switch (m_lights[i]->type)
		{
		case DIRECTIONAL_LIGHT:
			cb0->lights[i].Direction = XMVector3Transform(m_lights[i]->Direction, transpose);
			cb0->lights[i].Direction = XMVector3Normalize(cb0->lights[i].Direction);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			break;
		case POINT_LIGHT:
			cb0->lights[i].Position = XMVector3Transform(m_lights[i]->Position, inverse);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			cb0->lights[i].constAttenuation = m_lights[i]->constAttenuation;
			cb0->lights[i].linAttenuation = m_lights[i]->linAttenuation;
			cb0->lights[i].quadAttenuation = m_lights[i]->quadAttenuation;
			break;
		case SPOT_LIGHT:
			cb0->lights[i].Position = XMVector3Transform(m_lights[i]->Position, inverse);
			cb0->lights[i].Direction = XMVector3Transform(m_lights[i]->Direction, transpose);
			cb0->lights[i].Direction = XMVector3Normalize(cb0->lights[i].Direction);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			cb0->lights[i].constAttenuation = m_lights[i]->constAttenuation;
			cb0->lights[i].linAttenuation = m_lights[i]->linAttenuation;
			cb0->lights[i].quadAttenuation = m_lights[i]->quadAttenuation;
			cb0->lights[i].Angle = m_lights[i]->Angle;
			break;
		}
	}
}

void LightManager::renderLights(const XMMATRIX& modelMatrix, TEST_CONSTANT_BUFFER* cb0)
{
	XMMATRIX transpose = XMMatrixTranspose(modelMatrix);
	XMVECTOR determinant;
	XMMATRIX inverse = XMMatrixInverse(&determinant, modelMatrix);

	cb0->ambient_light_colour = m_ambientLight;
	cb0->lightCount = m_lights.size();
	cb0->InvTransWorldMatrix = inverse;

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		switch (m_lights[i]->type)
		{
		case DIRECTIONAL_LIGHT:
			cb0->lights[i].Direction = XMVector3Transform(m_lights[i]->Direction, transpose);
			cb0->lights[i].Direction = XMVector3Normalize(cb0->lights[i].Direction);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			break;
		case POINT_LIGHT:
			cb0->lights[i].Position = XMVector3Transform(m_lights[i]->Position, inverse);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			cb0->lights[i].constAttenuation = m_lights[i]->constAttenuation;
			cb0->lights[i].linAttenuation = m_lights[i]->linAttenuation;
			cb0->lights[i].quadAttenuation = m_lights[i]->quadAttenuation;
			break;
		case SPOT_LIGHT:
			cb0->lights[i].Position = XMVector3Transform(m_lights[i]->Position, inverse);
			cb0->lights[i].Direction = XMVector3Transform(m_lights[i]->Direction, transpose);
			cb0->lights[i].Direction = XMVector3Normalize(cb0->lights[i].Direction);
			cb0->lights[i].Colour = m_lights[i]->Colour;
			cb0->lights[i].type = m_lights[i]->type;
			cb0->lights[i].constAttenuation = m_lights[i]->constAttenuation;
			cb0->lights[i].linAttenuation = m_lights[i]->linAttenuation;
			cb0->lights[i].quadAttenuation = m_lights[i]->quadAttenuation;
			cb0->lights[i].Angle = m_lights[i]->Angle;
			break;
		}
	}
}

void LightManager::setAmbientLight(XMVECTOR colour)
{
	m_ambientLight = colour;
}

void LightManager::removeLight(int index)
{
	//TODO make sure this is correct
	m_lights.erase(m_lights.begin() + index);
}