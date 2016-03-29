TextureCube texture0;
SamplerState sampler0;

cbuffer CBuffer0
{
	matrix WVPMatrix; //64 bytes
};

struct VOut
{
	float4 position : SV_POSITION;
	float3 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
	VOut output;

	output.position = mul(WVPMatrix, position);

	output.texcoord = position.xyz;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float3 texcoord : TEXCOORD) : SV_TARGET
{
	return texture0.Sample(sampler0, texcoord);
}