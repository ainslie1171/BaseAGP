#ifndef _TEXT2D_H_
#define _TEXT2D_H_

#include "object2D.h"
#include <string.h>
#include <vector>

using namespace std;

struct String2D
{
	string text;
	float x, y, width, height, r, g, b;
	XMFLOAT4 colour;
	String2D(void) : x(0.0f), y(0.0f), width(0.0f), height(0.0f), r(0.0f), g(0.0f), b(0.0f)
	{
		colour.x = r;
		colour.y = g;
		colour.z = b;
		colour.w = 1.0f;
	};
};

class Text2D : Object2D
{
private:
	std::vector<String2D> m_string2Ds;
public:
	Text2D(char* filename, ID3D11Device* device, ID3D11DeviceContext* context);
	~Text2D(void);
	void addText(string text, float x, float y, float width, float height, float r, float g, float b);
	void addText(String2D* text);
	void Render(void);
};

#endif//_TEXT2D_H_

/*
Change Log:
-Added an alternitave addText function that will take a string2d pointer
*/