#include "mainMenu.h"

MainMenu::MainMenu(ID3D11Device* device, ID3D11DeviceContext* context, InputManager* input)
: Menu(device, context, input)
{
	m_title = "Main Menu";
	titleSize = 0.2f;

	MenuElement* m = new MenuElement(m_pD3DDevice, m_pImmediateContext);

	m->setText("assets/font1.bmp");
	String2D temp;
	temp.text = "Start Game";
	temp.width = 0.1f;
	temp.height = temp.width;
	temp.x = -((temp.text.length() / 2.0f)*temp.width);
	temp.y = 0.0f;
	temp.r = 0.8f;
	temp.g = 0.8f;
	temp.b = 0.8f;
	temp.colour = { temp.r, temp.g, temp.b, 0.0f };

	m->setPosition({ 0.0f, 0.2f, 0.0f });
	m->addText(&temp);

	m_elements.push_back(m);

	m = new MenuElement(m_pD3DDevice, m_pImmediateContext);
	temp.text = "Options";
	temp.x = -((temp.text.length() / 2.0f)*temp.width);

	m->setText("assets/font1.bmp");
	m->setPosition({ 0.0f, -0.15f, 0.0f });
	m->addText(&temp);
	m_elements.push_back(m);


	m = new MenuElement(m_pD3DDevice, m_pImmediateContext);
	temp.text = "Exit";
	temp.x = -((temp.text.length() / 2.0f)*temp.width);
	m->setText("assets/font1.bmp");
	m->setPosition({ 0.0f, -0.5f, 0.0f });
	m->addText(&temp);
	m_elements.push_back(m);

	m_selected = 0;
	m_elements[m_selected]->setSelected(true);
}

void MainMenu::Update(State& state)
{
	Menu::Update();

	if (m_pInputManager->IsKeyPressedOnce(DIK_RETURN))
		switch (m_selected)
	{
		case 0:
			//start
			state = STATE_GAME;
			break;
		case 1:
			//options
			state = STATE_OPTIONS;
			break;
		case 2:
			//exit
			state = STATE_QUIT;
			break;
	}
}

void MainMenu::Render()
{
	Menu::Render();
}