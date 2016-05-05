#ifndef _MENU_H_
#define _MENU_H_

#include "menuElement.h"
#include "inputManager.h"
#include "states.h"

//TODO MenuElement
//TODO Text2d
//TODO Sprite

class Menu
{
private:
	Text2D* m_pTitle;

protected:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	vector<MenuElement*> m_elements;
	std::string m_title;
	float titleSize;
	UINT m_selected;
	InputManager* m_pInputManager;
	bool SaveBack;
public:
	Menu(ID3D11Device* device, ID3D11DeviceContext* context, InputManager* input);// State* pState);
	~Menu(void);
	virtual void Update(void);
	virtual void Render(void);
	void setTitle(char* text);
	Text2D* getTitle(void);
};

#endif//_MENU_H_

/*
Change Log:
-changed some variable names to fit the naming convention
-added void to empty parameter lists for better readability
-using UINT instead of int
-changed set title to take the text for the title rather then the object
-added pointers to the device and device context (for menu element creation)
-added a pointer to the input manager so that menus can handle input
-added a reference to the state variale from DX11Framework in update's parameteres
-moved most the members to be protected (from private)
-added a variable for the text of the title
-the reference to state in update has been moved from this class to it's children classes
-added a variable for the size of the title
*/