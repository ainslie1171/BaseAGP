#ifndef _MENUELEMENT_H_
#define _MENUELEMENT_H_

#include "vector.h"
#include "sprite.h"
#include "text2D.h"

//TODO Sprite
//TODO Text2D
//TODO Vector3

class MenuElement
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	Text2D* m_pText;
	vector<Sprite*> m_sprite;
	vector<String2D*> m_string;
	Vector3 m_position;
	bool m_selected;
public:
	MenuElement(ID3D11Device* device, ID3D11DeviceContext* context);
	~MenuElement(void);
	void addSprite(char* filePath);
	void addText(String2D* text);
	void setText(char* filePath);
	void setPosition(Vector3 pos);
	vector<Sprite*> getSprites(void);
	vector<Text2D*> getText(void);
	Vector3 getPosition(void);
	void Render(void);
	void setSelected(bool s);
};

#endif//_MENUELEMENT_H_

/*
Change Log:
-added void to empty parameter lists for better readability
-swapped use of XMVECTOR for Vector3
-added pointers to the device and device context (for text and sprite creation)
-changed addText parameter to take a String2D
-added a bool m_selected so that the element can know if it is selected or not
*/