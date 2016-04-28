#ifndef _OPTIONSMENU_H_
#define _OPTIONSMENU_H_

#include "menu.h"

class OptionsMenu : public Menu
{
private:
	Text2D* saveText;
	Text2D* backText;
	String2D* saveString;
	String2D* backString;

public:
	OptionsMenu(ID3D11Device* device, ID3D11DeviceContext* context, InputManager* input);
	void Update(State& state);
	void Render(void);
};

#endif//_OPTIONSMENU_H_

/*
Change Log:

*/