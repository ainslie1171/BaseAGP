#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "object2D.h"


class Sprite : Object2D
{
private:
	void setVertexPos(POS_TEX_COL_VERTEX* pVertex, float x, float y);
	void setVertexTexture(POS_TEX_COL_VERTEX* pVertex, float texX, float texY);

public:
	Sprite(char* filename, ID3D11Device* device, ID3D11DeviceContext* context);
	~Sprite(void);
	void defineVerticies(float posX, float posY, float height, float width);
	void defineVertexPositions(float posX, float posY, float height, float width);
	void Render(void);
};

#endif//_SPRITE_H_

/*
Change Log:

*/