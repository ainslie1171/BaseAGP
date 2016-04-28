#include "object2D.h"


Object2D::Object2D(char* filename, ID3D11Device* device, ID3D11DeviceContext* context, int sizeofVertexArr)
{
	m_pD3DDevice = device;
	m_pImmediateContext = context;

	//TODO
	UNREFERENCED_PARAMETER(sizeofVertexArr);

	//m_pVertices = new POS_TEX_VERTEX[sizeofVertexArr];

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;										// Used by CPU and GPU
	bufferDesc.ByteWidth = sizeof(m_pVertices);									// Total size of buffer,
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// Use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// Allow CPU access
	HRESULT hr = m_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);	// Create the buffer

	if (FAILED(hr)) exit(0);


	//I might take the shaders out of here later
	
	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("text2d_shaders.hlsl", 0, 0, "TextVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))exit(0);
	}

	hr = D3DX11CompileFromFile("text2d_shaders.hlsl", 0, 0, "TextPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) exit(0);
	}

	// Create shader objects
	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);
	if (FAILED(hr)) exit(0);

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);
	if (FAILED(hr)) exit(0);

	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr)) exit(0);
	
	
	// Load in the font texture from given filename
	hr = D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, filename, NULL, NULL, &m_pTexture, NULL);
	if (FAILED(hr)) exit(0);

	// Create sampler for texture
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler);

	// Create 2 depth stencil states to turn Z buffer on and off
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = m_pD3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthDisabledStencilState);
	if (FAILED(hr)) exit(0);

	depthStencilDesc.DepthEnable = true;
	hr = m_pD3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthEnabledStencilState);
	if (FAILED(hr)) exit(0);
}

Object2D::~Object2D(void)
{

}

void Object2D::MapSubResouce(int size)
{
	//I WAS HERE
	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// Lock the buffer to allow writing
	memcpy(ms.pData, m_pVertices, sizeof(m_pVertices[0])* size * 6);						// Copy the data -  only upload those that are used
	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);
}

void Object2D::SetRenderStates(void)
{
	// set all rendering states
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture);
	//TODO Shaders
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	UINT stride = sizeof(POS_TEX_COL_VERTEX);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}