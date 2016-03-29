
TextureCube texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0;

#define MAX_LIGHTS 10
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct light
{
	float4 position;
	float4 direction;
	float4 colour;
	float angle;
	float constAttenuation;
	float linAttenuation;
	float quadAttenuation;
	int type;
	int3 padding;
};

struct material
{
	float4 Emissive;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float specularPower;
	bool useTexture;
	int2 padding;
};

cbuffer CBuffer0 : register(b0)
{
	matrix WMatrix;
    matrix WVMatrix;
	matrix ITWMatrix;
	matrix WVPMatrix;
	float4 cameraPosition;
	float4 ambient_light_colour;
	light lights[MAX_LIGHTS];
	int lightCount;
	int3 padding;
	material Material;
};

struct VOut
{
	float4 position : SV_POSITION;
    float4 PositionWS : WS_POSITION;
    float3 NormalWS : WS_NORMAL;
	float2 texcoord : TEXCOORD;
    float3 texcoord2 : TEXCOORD2;
    float3 eye : EYE;
};

VOut ModelVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	output.position = mul(WVPMatrix, position);

    output.PositionWS = mul(WMatrix, position);

    output.NormalWS = mul((float3x3) ITWMatrix, normal);

    output.texcoord = texcoord;

    float3 wvpos = mul(WVMatrix, position);
    float3 wvnormal = mul(WVMatrix, normal);
    wvnormal = normalize(wvnormal);
    float3 eyer = -normalize(wvpos);
    output.texcoord2 = 2.0 * dot(eyer, wvnormal) * wvnormal - eyer;

	return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 PositionWS : WS_POSITION, float3 NormalWS : WS_NORMAL, float2 texcoord : TEXCOORD, float3 texcoord2 : TEXCOORD2, float3 eye : EYE) : SV_TARGET
{

    float3 normal = normalize(NormalWS);
    
    float3 wvpos = mul(WVMatrix, PositionWS);
    float3 wvnormal = mul(WVMatrix, normal);
    wvnormal = normalize(wvnormal);
    float3 eyer = normalize(-wvpos);


    float distance, attenuation, minCos, maxCos, cosAngle, intensity, colour;
    //float3 viewVector = normalize(cameraPosition - PositionWS).xyz,
    float3 viewVector = normalize(cameraPosition - PositionWS).xyz,
		lightVector,
		reflectVector;
    float4 diffuse_amount = { 0.0f, 0.0f, 0.0f, 0.0f },
		specular_amount = { 0.0f, 0.0f, 0.0f, 0.0f };

	//calculate lighting
    for (int i = 0; i < lightCount; i++)
    {
        switch (lights[i].type)
        {
            case DIRECTIONAL_LIGHT:
            //diffuse
            lightVector = -lights[i].direction.xyz;
            diffuse_amount += lights[i].colour * max(0, dot(normal, lightVector));
			 
            //specular   
            //reflectVector = normalize(reflect(-lightVector, normal));
                //specular_amount += lights[i].colour * pow(max(0, dot(reflectVector, eyer)), Material.specularPower);
                reflectVector = normalize(lightVector + viewVector);
                specular_amount += lights[i].colour * pow(max(0, dot(NormalWS, reflectVector)), Material.specularPower);
                break;

            case POINT_LIGHT:
            //diffuse
            lightVector = (lights[i].position - position).xyz;
            distance = length(lightVector);
            lightVector = lightVector / distance;
            attenuation = 1.0f / (lights[i].constAttenuation + lights[i].linAttenuation * distance + lights[i].quadAttenuation * distance * distance);
            diffuse_amount += (lights[i].colour * max(0, dot(NormalWS, lightVector))) * attenuation;
           
            //specular     
                reflectVector = normalize(lightVector + viewVector);
                specular_amount += (lights[i].colour * pow(max(0, dot(NormalWS, reflectVector)), Material.specularPower)) * attenuation;
                break;

            case SPOT_LIGHT:
            //diffuse
            lightVector = (lights[i].position - position).xyz;
            distance = length(lightVector);
            lightVector = lightVector / distance;
            attenuation = 1.0f / (lights[i].constAttenuation + lights[i].linAttenuation * distance + lights[i].quadAttenuation * distance * distance);
            minCos = cos(lights[i].angle);
            maxCos = (minCos + 1.0f) / 2.0f;
            cosAngle = dot(lights[i].direction.xyz, -lightVector);
            intensity = smoothstep(minCos, maxCos, cosAngle);
            diffuse_amount += (lights[i].colour * max(0, dot(normal, lightVector))) * attenuation * intensity;
            
            //specular
                reflectVector = normalize(lightVector + viewVector);
                specular_amount += (lights[i].colour * pow(max(0, dot(NormalWS, reflectVector)), Material.specularPower)) * attenuation * intensity;
                break;
        }
    }

	//calculate material values
    float4 emissive = Material.Emissive;
    float4 ambient = Material.Ambient * ambient_light_colour;
    float4 diffuse = Material.Diffuse * diffuse_amount;
    float4 specular = Material.Specular * specular_amount;
    
    colour = ambient + diffuse + specular + emissive;

    









	//calculate texture against material
	float4 textureColour = { 1, 1, 1, 1 };

    if (Material.useTexture)
    {
        textureColour = texture1.Sample(sampler0, texcoord);
    }

    float4 reflectionColor = texture0.Sample(sampler0, texcoord2);

    float shiny = (Material.specularPower - 10) / (100 - 10);

    textureColour = lerp(textureColour, reflectionColor, shiny);

	//output colour

	return colour * textureColour;
}