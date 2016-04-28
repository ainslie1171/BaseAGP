#include "menu.h"

Menu::Menu(ID3D11Device* device, ID3D11DeviceContext* context, InputManager* input)
{
	m_pD3DDevice = device;
	m_pImmediateContext = context;
	m_pInputManager = input;

	m_pTitle = new Text2D("assets/font1.bmp", m_pD3DDevice, m_pImmediateContext);
}

Menu::~Menu()
{
	while (!m_elements.empty())
	{
		delete m_elements.back();
		m_elements.pop_back();
	}
	if (m_pTitle) delete m_pTitle; m_pTitle = nullptr;
}

void Menu::Update(void)
{
	if (m_pInputManager->IsKeyPressedOnce(DIK_DOWN))
	{
		m_elements[m_selected]->setSelected(false);
		if (m_selected < m_elements.size()-1)
			m_selected++;
		m_elements[m_selected]->setSelected(true);
	}
	else if (m_pInputManager->IsKeyPressedOnce(DIK_UP))
	{
		m_elements[m_selected]->setSelected(false);
		if (m_selected > 0)
			m_selected--;
		m_elements[m_selected]->setSelected(true);
	}
}

void Menu::Render()
{
	m_pTitle->addText(m_title, -((m_title.length() / 2.0f)*titleSize), 0.8f, titleSize, titleSize, 1.0f, 1.0f, 1.0f);
	m_pTitle->Render();
	
	for (MenuElement* m : m_elements)
		m->Render();
	
}

void Menu::setTitle(char* text)
{
	UNREFERENCED_PARAMETER(text);
}

Text2D* Menu::getTitle()
{
	return m_pTitle;
}