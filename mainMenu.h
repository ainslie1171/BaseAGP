#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "menu.h"

class MainMenu : public Menu
{
private:

public:
	MainMenu(ID3D11Device* device, ID3D11DeviceContext* context, InputManager* input);
	void Update(State& state);
	void Render(void);
};

#endif//_MAINMENU_H_

/*
Change Log:

*/