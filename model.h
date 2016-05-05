#ifndef _MODEL_H_
#define _MODEL_H_

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;


class Model
{
private:
	ID3D11Device*           pD3DDevice;
	ID3D11DeviceContext*    pImmediateContext;
	UINT					ID;


//////////////////////////////////////////////////

	int loadfile(char* fname);

	char* fbuffer;
	long fbuffersize; // filesize
	size_t actualsize; // actual size of loaded data (can be less if loading as text files as ASCII CR (0d) are stripped out)

//////////////////////////////////////////////////

	void parsefile();
	bool getnextline() ;
	bool getnexttoken(int& tokenstart, int& tokenlength);

	unsigned int  tokenptr;

//////////////////////////////////////////////////

	bool createVB();

	ID3D11Buffer* pVertexBuffer; 

public:

	struct xyz { float x, y, z; };	//used for vertices and normals during file parse
	struct xy { float x, y; };		//used for texture coordinates during file parse

	// Define model vertex structure
	struct MODEL_POS_TEX_NORM_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};


	string filename;

	Model(char* filename, ID3D11Device* device, ID3D11DeviceContext* context, UINT ID);
	~Model();

	void Draw(void);

	vector <xyz> position_list;		// list of parsed positions
	vector <xyz> normal_list;		// list of parsed normals
	vector <xy> texcoord_list;		// list of parsed texture coordinates
	vector <int> pindices, tindices, nindices; // lists of indicies into above lists derived from faces

	UINT getID() { return ID; }

	MODEL_POS_TEX_NORM_VERTEX* vertices;
	unsigned int numverts;
};



#endif//_MODEL_H_