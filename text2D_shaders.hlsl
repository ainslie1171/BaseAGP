Texture2D texture0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
	float2 texcoord :TEXCOORD;
    float4 colour : COLOUR;
};

VOut TextVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float4 colour : COLOUR)
{
	VOut output;

	output.position = position;

	output.texcoord = texcoord;

    output.colour = colour;

	return output;
}


float4 TextPS(float4 position : SV_POSITION, float2 texcoord : TEXCOORD, float4 colour : COLOUR) : SV_TARGET
 {
	return colour * texture0.Sample(sampler0, texcoord);
 }