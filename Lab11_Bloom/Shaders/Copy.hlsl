#include "Constants.hlsl"

struct VIn
{
	float3 position : POSITION0;
	float2 texturePos : TEXCOORD0;

};

struct VOut
{
	float4 position : SV_POSITION;
	float2 texturePos : TEXCOORD0;

};

VOut VS(VIn vIn)
{
	VOut output;
	output.texturePos = vIn.texturePos;

	output.position = float4(vIn.position, 1.0);
	
	return output;

}

float4 PS(VOut pIn) : SV_TARGET
{
	float4 diffuse = DiffuseTexture.Sample(DefaultSampler, pIn.texturePos);

	return diffuse;

}