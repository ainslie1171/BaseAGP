#include"debugLine.h"
#include "DX11Framework.h"
DebugLine* DebugLine::m_pInstance = new DebugLine;


DebugLine* DebugLine::getInstance(void)
{
	if (!m_pInstance) m_pInstance = new DebugLine;
	return m_pInstance;
}



void DebugLine::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	pD3DDevice = device;
	pImmediateContext = context;

	//set up vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;				//Used by CPU and GPU
	bufferDesc.ByteWidth = sizeof(vertices);			//Total size of buffer
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//Use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //Allow CPU access 

	//Create the vertex buffer
	device->CreateBuffer(&bufferDesc, NULL, &pVertexBuffer);

	//setup the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //Can use UpdateSubresourse() to update
	constant_buffer_desc.ByteWidth = 80; //MUST be a multiple of 16, calculated from the constant buffer struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //Use as a constant buffer

	//create the constant buffer
	device->CreateBuffer(&constant_buffer_desc, NULL, &pConstantBuffer);

	//Load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS,
		*PS,
		*error;
	D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	//check for shader compilation error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
	}

	D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	//check for shader compilation error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
	}

	//Create shader objects
	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);



	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);


	//Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
}

void DebugLine::draw(Vector3 PosA, Vector3 PosB, Vector3 colour)
{
	vertices[0] = PosA;
	vertices[1] = PosB;
	vertices[2] = PosA;

	Vector4 col = colour;
	col.w = 1.0f;
	cbValues.colour = col.getXMFloat4();
}

void DebugLine::Render(XMMATRIX wvp)
{
	cbValues.WorldViewProjection = wvp;

	//copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//lock the buffer to allow writting
	pImmediateContext->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	//copy the data
	memcpy(ms.pData, vertices, sizeof(vertices));

	//unlock the buffer
	pImmediateContext->Unmap(pVertexBuffer, NULL);

	//set the shader objects as active
	pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	pImmediateContext->IASetInputLayout(m_pInputLayout);
	DX11Framework::getInstance()->setRSState(RS_WIREFRAME);

	//upload the new values for the constant buffer
	pImmediateContext->UpdateSubresource(pConstantBuffer, 0, 0, &cbValues, 0, 0);
	//set the constant buffer to be active
	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

	UINT stride = sizeof(Vector3),
		offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	pImmediateContext->Draw(3, 0);
}

/*

Maybe put this in its own function

//copy the vertices into the buffer
D3D11_MAPPED_SUBRESOURCE ms;

//lock the buffer to allow writting
g_pImmediateContext->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

//copy the data
memcpy(ms.pData, vertices, sizeof(vertices));

//unlock the buffer
g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);

*/