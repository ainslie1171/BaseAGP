#include "plane.h"

p_Plane::p_Plane(ID3D11Device* device, ID3D11DeviceContext* context, Vector3 pos, Vector3 rot, float s)
{
	rotation = rot;
	position = pos;
	scale = s;


	m_pVertices[0] = { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	m_pVertices[1] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	m_pVertices[2] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	m_pVertices[3] = { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	m_pVertices[4] = { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	m_pVertices[5] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };


	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;										// Used by CPU and GPU
	bufferDesc.ByteWidth = sizeof(m_pVertices);									// Total size of buffer,
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// Use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// Allow CPU access
	device->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);					// Create the buffer

	ID3DBlob *VS, *PS, *error;
	D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);


	// Create shader objects
	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);


	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);


	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	m_pImmediateContext = context;

	//setup the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //Can use UpdateSubresourse() to update
	constant_buffer_desc.ByteWidth = sizeof(PlaneBuffer); //MUST be a multiple of 16, calculated from the constant buffer struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //Use as a constant buffer

	//create the constant buffer
	device->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	
	//copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//lock the buffer to allow writting
	context->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	//copy the data
	memcpy(ms.pData, m_pVertices, sizeof(m_pVertices));

	//unlock the buffer
	context->Unmap(m_pVertexBuffer, NULL);
}

p_Plane::~p_Plane()
{
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pInputLayout)  m_pInputLayout->Release();
	if (m_pVShader) m_pVShader->Release();
	if (m_pPShader)  m_pPShader->Release();
}

void p_Plane::draw(XMMATRIX vp)
{
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	XMVECTOR r;
	r.x = XMConvertToRadians(rotation.x);
	r.y = XMConvertToRadians(rotation.y);
	r.z = XMConvertToRadians(rotation.z);

	XMMATRIX w = XMMatrixScaling(scale, scale, scale);
	w *= XMMatrixRotationRollPitchYawFromVector(r);
	w *= XMMatrixTranslationFromVector(position.getXMVector());

	PlaneBuffer pb;
	pb.WorldViewProjection = w * vp;


	UINT stride = sizeof(POS_COL_VERTEX),
		offset = 0;

	//upload the new values for the constant buffer
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &pb, 0, 0);

	//set the constant bugger to be active
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pImmediateContext->Draw(6, 0);
}

void p_Plane::setPos(Vector3 pos)
{
	position = pos;
}

void p_Plane::setRot(Vector3 rot)
{
	rotation = rot;
}