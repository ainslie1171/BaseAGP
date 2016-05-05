#include "menuElement.h"

MenuElement::MenuElement(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_pD3DDevice = device;
	m_pImmediateContext = context;
}

MenuElement::~MenuElement(void)
{
	if (m_pText) delete m_pText; m_pText = nullptr;

	while (!m_sprite.empty())
	{
		delete m_sprite.back();
		m_sprite.pop_back();
	}
	while (!m_string.empty())
	{
		delete m_string.back();
		m_string.pop_back();
	}
}

void MenuElement::addSprite(char* filePath)
{
	Sprite* temp = new Sprite(filePath, m_pD3DDevice, m_pImmediateContext);
	m_sprite.push_back(temp);
}

void MenuElement::addText(String2D* text)
{
	String2D* s = new String2D();
	*s = *text;
	s->x += m_position.x;
	s->y += m_position.y;
	m_string.push_back(s);
}

void MenuElement::setText(char* filePath)
{
	m_pText = new Text2D(filePath, m_pD3DDevice, m_pImmediateContext);
}

void MenuElement::setPosition(Vector3 pos)
{
	m_position = pos;
}

vector<Sprite*> MenuElement::getSprites(void)
{
	return m_sprite;
}

vector<Text2D*> MenuElement::getText(void)
{
	//return m_text;
	vector<Text2D*> s;
	return s;
}

Vector3 MenuElement::getPosition(void)
{
	return m_position;
}

void MenuElement::Render(void)
{
	/*
	for (Sprite* s : m_sprite)
		s->Render();
	*/

	for (String2D* t : m_string)
	{
		if (m_selected)
		{
			String2D temp = *t;
			temp.colour.x /= 4.0f;
			temp.colour.y /= 4.0f;
			temp.colour.z /= 4.0f;
			m_pText->addText(&temp);
		}
		else
		{
			m_pText->addText(t);
		}
		m_pText->Render();
	}
}

void MenuElement::setSelected(bool s)
{
	m_selected = s;
}