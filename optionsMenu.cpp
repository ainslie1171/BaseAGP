#include "optionsMenu.h"

OptionsMenu::OptionsMenu(ID3D11Device* device, ID3D11DeviceContext* context, InputManager* input)
: Menu(device, context, input)
{
	m_title = "Options Menu";
	titleSize = 0.16f;

	MenuElement* m = new MenuElement(m_pD3DDevice, m_pImmediateContext);

	m->setText("assets/font1.bmp");
	String2D temp;
	temp.text = "Fullscreen";
	temp.width = 0.1f;
	temp.height = temp.width;
	temp.x = -((temp.text.length() / 2.0f)*temp.width);
	temp.y = 0.0f;
	temp.r = 0.8f;
	temp.g = 0.8f;
	temp.b = 0.8f;
	temp.colour = { temp.r, temp.g, temp.b, 0.0f };

	m->setPosition({ 0.0f, 0.3f, 0.0f });
	m->addText(&temp);

	m_elements.push_back(m);

	m = new MenuElement(m_pD3DDevice, m_pImmediateContext);
	temp.text = "Music Volume";
	temp.x = -((temp.text.length() / 2.0f)*temp.width);

	m->setText("assets/font1.bmp");
	m->setPosition({ 0.0f, 0.1f, 0.0f });
	m->addText(&temp);
	m_elements.push_back(m);


	m = new MenuElement(m_pD3DDevice, m_pImmediateContext);
	temp.text = "SFX Volume";
	temp.x = -((temp.text.length() / 2.0f)*temp.width);
	m->setText("assets/font1.bmp");
	m->setPosition({ 0.0f, -0.1f, 0.0f });
	m->addText(&temp);
	m_elements.push_back(m);

	m = new MenuElement(m_pD3DDevice, m_pImmediateContext);
	temp.text = "Resolution";
	temp.x = -((temp.text.length() / 2.0f)*temp.width);
	m->setText("assets/font1.bmp");
	m->setPosition({ 0.0f, -0.3f, 0.0f });
	m->addText(&temp);
	m_elements.push_back(m);

	m = new MenuElement(m_pD3DDevice, m_pImmediateContext);
	temp.text = "FOV";
	temp.x = -((temp.text.length() / 2.0f)*temp.width);
	m->setText("assets/font1.bmp");
	m->setPosition({ 0.0f, -0.5f, 0.0f });
	m->addText(&temp);
	m_elements.push_back(m);

	m = new MenuElement(m_pD3DDevice, m_pImmediateContext);
	temp.text = "";
	temp.x = -((temp.text.length() / 2.0f)*temp.width);
	m->setText("assets/font1.bmp");
	m->setPosition({ 0.0f, -0.8f, 0.0f });
	m->addText(&temp);
	m_elements.push_back(m);

	temp.text = "Save";
	temp.x = -((std::string("Save    Back").length() / 2.0f)*temp.width);
	temp.y = -0.8f;
	saveText = new Text2D("assets/font1.bmp", m_pD3DDevice, m_pImmediateContext);
	saveString = new String2D(temp);

	temp.text = "Back";
	temp.x = ((std::string("Save").length() / 2.0f)*temp.width);
	backText = new Text2D("assets/font1.bmp", m_pD3DDevice, m_pImmediateContext);
	backString = new String2D(temp);
	m_selected = 0;
	m_elements[m_selected]->setSelected(true);
}

void OptionsMenu::Update(State& state)
{
	Menu::Update();

	switch (m_selected)
	{
		case 0:
			//fullscreen

			break;
		case 1:
			//Music volume

			break;
		case 2:
			//sfx volume

			break;
		case 3:
			//resolution

			break;
		case 4: 
			//FOV

			break;

		case 5:
			//save back
			if (SaveBack)
			{
				if (m_pInputManager->IsKeyPressedOnce(DIK_RIGHT))
					SaveBack = false;

				if (m_pInputManager->IsKeyPressedOnce(DIK_RETURN)) {}
					//save settings
			}
			else
			{
				if (m_pInputManager->IsKeyPressedOnce(DIK_LEFT))
					SaveBack = true;

				if (m_pInputManager->IsKeyPressedOnce(DIK_RETURN))
					state = STATE_MENU;
			}
			break;
	}

	//if (m_pInputManager->IsKeyPressedOnce(DIK_RETURN))
}

void OptionsMenu::Render()
{
	Menu::Render();

	if (m_selected == 5)
	{
		if (SaveBack)
		{
			String2D temp = *saveString;
			temp.colour.x /= 4.0f;
			temp.colour.y /= 4.0f;
			temp.colour.z /= 4.0f;
			saveText->addText(&temp);
			backText->addText(backString);

		}
		else
		{
			String2D temp = *backString;
			temp.colour.x /= 4.0f;
			temp.colour.y /= 4.0f;
			temp.colour.z /= 4.0f;
			backText->addText(&temp);
			saveText->addText(saveString);
		}
	}
	else
	{
		saveText->addText(saveString);
		backText->addText(backString);
	}
	
	saveText->Render();
	backText->Render();
}