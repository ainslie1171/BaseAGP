cbuffer CBuffer0
{
    matrix wvp;
    float4 colour;
};

struct VOut
{
	float4 position : SV_POSITION;
    float4 vColour : COLOUR;
	float2 texcoord :TEXCOORD;
};

VOut VShader(float4 position : POSITION)
{
	VOut output;

    output.position = mul(wvp, position);
    output.vColour = colour;
    output.texcoord = position.xy;

	return output;
}


 float4 PShader(float4 position : SV_POSITION, float4 vColour: COLOUR, float2 texcoord : TEXCOORD) : SV_TARGET 
 {
    float distSq = (texcoord.x * texcoord.x) + (texcoord.y * texcoord.y);
    clip(1.0f - distSq);
    return vColour;
}