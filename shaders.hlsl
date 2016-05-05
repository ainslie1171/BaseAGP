cbuffer CBuffer0
{
    float4 colour;
    matrix WVPMatrix; //64 bytes
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(float4 position : POSITION)
{
    VOut output;

    output.position = mul(WVPMatrix, position);
    output.color = colour;

    return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}