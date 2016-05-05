#include "text2D.h"

Text2D::Text2D(char* filename, ID3D11Device* device, ID3D11DeviceContext* context)
:Object2D(filename, device, context, 10000)
{

}

Text2D::~Text2D(void)
{
	if (m_pDepthDisabledStencilState) m_pDepthDisabledStencilState->Release();
	if (m_pDepthEnabledStencilState) m_pDepthEnabledStencilState->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
}

void Text2D::addText(string text, float x, float y, float width, float height, float r, float g, float b)
{
	String2D temp;
	temp.text = text;
	temp.x = x;
	temp.y = y;
	temp.width = width;
	temp.height = height;

	//Might have to change this later
	temp.r = r;
	temp.b = b;
	temp.g = g;
	temp.colour = { r, g, b, 1.0f };

	m_string2Ds.push_back(temp);
}

void Text2D::addText(String2D* text)
{
	String2D temp = *text;
	m_string2Ds.push_back(temp);
}

void Text2D::Render(void)
{
	int current_char = 0; // keep track of number of characters so far

	// loop through all the added string_2d
	for (unsigned int i = 0; i < m_string2Ds.size(); i++)
	{
		float tempx = m_string2Ds[i].x;
		float tempy = m_string2Ds[i].y;
		float temph = m_string2Ds[i].height;
		float tempw = m_string2Ds[i].width;
		XMFLOAT4 tempc = m_string2Ds[i].colour;

		// loop through each character
		for (unsigned int j = 0; j < m_string2Ds[i].text.length(); j++)
		{
			// create 6 vertices with given size and xy position
			m_pVertices[current_char * 6].Pos.x = tempx;
			m_pVertices[current_char * 6].Pos.y = tempy;
			m_pVertices[current_char * 6 + 1].Pos.x = tempx + tempw;
			m_pVertices[current_char * 6 + 1].Pos.y = tempy;
			m_pVertices[current_char * 6 + 2].Pos.x = tempx + tempw;
			m_pVertices[current_char * 6 + 2].Pos.y = tempy - temph;
			m_pVertices[current_char * 6 + 3].Pos.x = tempx;
			m_pVertices[current_char * 6 + 3].Pos.y = tempy;
			m_pVertices[current_char * 6 + 4].Pos.x = tempx + tempw;
			m_pVertices[current_char * 6 + 4].Pos.y = tempy - temph;
			m_pVertices[current_char * 6 + 5].Pos.x = tempx;
			m_pVertices[current_char * 6 + 5].Pos.y = tempy - temph;

			// set all z to 1.0 to avoid being clipped
			m_pVertices[current_char * 6].Pos.z = 1.0;
			m_pVertices[current_char * 6 + 1].Pos.z = 1.0;
			m_pVertices[current_char * 6 + 2].Pos.z = 1.0;
			m_pVertices[current_char * 6 + 3].Pos.z = 1.0;
			m_pVertices[current_char * 6 + 4].Pos.z = 1.0;
			m_pVertices[current_char * 6 + 5].Pos.z = 1.0;

			// this code assumes 26 characters across and 4 lines in texture, containing lower, upper, number, symbols
			const int NUMLINES = 4;

			char c = m_string2Ds[i].text[j]; //  get current character

			float texy, texx; // temp tex coords

			// determine texture coord to begin at, based on character
			// 1/26th added to x texture coord for each subsequent letter in alhpabet
			if (c >= 'a' && c <= 'z') // lowercase
			{
				texy = 0.0; // first line
				texx = (c - 'a') * 1.0f / 26.0f;
			}
			else if (c >= 'A' && c <= 'Z') // uppercase
			{
				texy = 1.0 / NUMLINES; //second line
				texx = (c - 'A') *1.0f / 26.0f;
			}
			else if (c >= '0' && c <= '9') // numbers
			{
				texy = 2.0 / NUMLINES; // third line
				texx = (c - '0') *1.0f / 26.0f;
			}
			else if (c >= ' ' && c <= '/') // add any symbol code here
			{
				texy = 3.0 / NUMLINES;
				texx = (c - ' ')*1.0f / 26.0f;
			}
			else if (c >= ':' && c <= '?')
			{
				texy = 3.0 / NUMLINES;
				texx = ((c - ':') + 16)*1.0f / 26.0f;
			}
			else
			{
				texy = 3.0f / NUMLINES;
				texx = 0.0f;
			}


			// set correct texture coordinates for letter
			m_pVertices[current_char * 6].Texture.x = texx;
			m_pVertices[current_char * 6].Texture.y = texy;
			m_pVertices[current_char * 6 + 1].Texture.x = texx + 1.0f / 26.0f;
			m_pVertices[current_char * 6 + 1].Texture.y = texy;
			m_pVertices[current_char * 6 + 2].Texture.x = texx + 1.0f / 26.0f;
			m_pVertices[current_char * 6 + 2].Texture.y = texy + 1.0f / NUMLINES;
			m_pVertices[current_char * 6 + 3].Texture.x = texx;
			m_pVertices[current_char * 6 + 3].Texture.y = texy;
			m_pVertices[current_char * 6 + 4].Texture.x = texx + 1.0f / 26.0f;
			m_pVertices[current_char * 6 + 4].Texture.y = texy + 1.0f / NUMLINES;
			m_pVertices[current_char * 6 + 5].Texture.x = texx;
			m_pVertices[current_char * 6 + 5].Texture.y = texy + 1.0f / NUMLINES;

			m_pVertices[current_char * 6].Colour = tempc;
			m_pVertices[current_char * 6 + 1].Colour = tempc;
			m_pVertices[current_char * 6 + 2].Colour = tempc;
			m_pVertices[current_char * 6 + 3].Colour = tempc;
			m_pVertices[current_char * 6 + 4].Colour = tempc;
			m_pVertices[current_char * 6 + 5].Colour = tempc;

			current_char++;
			tempx += tempw; // position next character along in x 
		}
	}
	
	// clear out the vector every frame, otherwise will grow forever
	m_string2Ds.clear();



	MapSubResouce(current_char);

	SetRenderStates();

	// turn off Z buffer so text always on top
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);

	// draw all added characters
	m_pImmediateContext->Draw(current_char * 6, 0);

	// turn on Z buffer so other rendering can use it
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthEnabledStencilState, 1);
}