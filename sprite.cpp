#include "sprite.h"

Sprite::Sprite(char* filename, ID3D11Device* device, ID3D11DeviceContext* context)
: Object2D(filename, device, context, 10000)
{

}

Sprite::~Sprite(void)
{
	if (m_pDepthDisabledStencilState) m_pDepthDisabledStencilState->Release();
	if (m_pDepthEnabledStencilState) m_pDepthEnabledStencilState->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
}

void Sprite::defineVerticies(float posX, float posY, float height, float width)
{
	UNREFERENCED_PARAMETER(posX);
	UNREFERENCED_PARAMETER(posY);
	UNREFERENCED_PARAMETER(height);
	UNREFERENCED_PARAMETER(width);
}

void Sprite::defineVertexPositions(float posX, float posY, float height, float width)
{
	UNREFERENCED_PARAMETER(posX);
	UNREFERENCED_PARAMETER(posY);
	UNREFERENCED_PARAMETER(height);
	UNREFERENCED_PARAMETER(width);
}

void Sprite::Render(void)
{
	//TODO
	/*
	//MapSubResouce(current_char);

	SetRenderStates();

	// turn off Z buffer so text always on top
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);

	// draw all added characters
	m_pImmediateContext->Draw(m_sizeOfVertexArr * 6, 0);

	// turn on Z buffer so other rendering can use it
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthEnabledStencilState, 1);
	*/
}
	
void Sprite::setVertexPos(POS_TEX_COL_VERTEX* pVertex, float x, float y)
{
	UNREFERENCED_PARAMETER(pVertex);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
}
	
void Sprite::setVertexTexture(POS_TEX_COL_VERTEX* pVertex, float texX, float texY)
{
	UNREFERENCED_PARAMETER(pVertex);
	UNREFERENCED_PARAMETER(texX);
	UNREFERENCED_PARAMETER(texY);
}
