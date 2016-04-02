Texture2D texture0;
SamplerState sampler0;

struct light
{
	float4 vec;
	float4 col;
};

cbuffer CBuffer0
{
	float red_fraction; //4 bytes
	float scale; //4 bytes
	float2 packing; //2 x 4 bytes
	matrix WVPMatrix; //64 bytes
	float4 ambient_light_colour; //16 bytes
	light directional_lights[5];
	light point_lights[5];
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	//Position
	/*position.x *= scale;
	position.y *= (1.0 - scale);
	output.position.xy *= scale;*/
	output.position = mul(WVPMatrix, position);

	//Colour
	color.r *= red_fraction;
	output.color = color;

	float	directionalColour = 0,
			diffuse_amount = 0,
			point_amount = 0,
			point_colour = 0;
	float4	lightVector;

	//Lighting
	
	for (int i = 0; i < 5; i++)
	{
		diffuse_amount = dot(directional_lights[i].vec, normal);
		diffuse_amount = saturate(diffuse_amount);
		directionalColour += (directional_lights[i].col * diffuse_amount);
	}
	for (int i = 0; i < 5; i++)
	{
		lightVector = point_lights[i].vec - position;
		point_amount = dot(normalize(lightVector), normal);
		point_colour += (point_lights[i].col * saturate(point_amount));
	}
	/*
	float diffuse_amount1 = dot(directional_lights[0].vec, normal);
	diffuse_amount1 = saturate(diffuse_amount1);
	directionalColour = (directional_lights[0].col * diffuse_amount1);

	float diffuse_amount2 = dot(directional_lights[1].vec, normal);
	diffuse_amount2 = saturate(diffuse_amount2);
	directionalColour += (directional_lights[1].col * diffuse_amount2);

	float diffuse_amount3 = dot(directional_lights[2].vec, normal);
	diffuse_amount3 = saturate(diffuse_amount3);
	directionalColour += (directional_lights[2].col * diffuse_amount3);

	float diffuse_amount4 = dot(directional_lights[3].vec, normal);
	diffuse_amount4 = saturate(diffuse_amount4);
	directionalColour += (directional_lights[3].col * diffuse_amount4);

	float diffuse_amount5 = dot(directional_lights[4].vec, normal);
	diffuse_amount5 = saturate(diffuse_amount5);
	directionalColour += (directional_lights[4].col * diffuse_amount5);
	*/

	output.color = ambient_light_colour + directionalColour + point_colour;

	//Texture
	output.texcoord = texcoord;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * texture0.Sample(sampler0, texcoord);
}