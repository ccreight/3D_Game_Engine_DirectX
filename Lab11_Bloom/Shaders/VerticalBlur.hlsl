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

	float offset[] = { 0.0, 1.3846153846, 3.2307692308 };
	float weight[] = { 0.2270270270, 0.3162162162, 0.0702702703 };

	float4 fragColor = DiffuseTexture.Sample(DefaultSampler, pIn.texturePos) * weight[0];

	for (int i = 1; i < 3; i++)
	{
		fragColor += DiffuseTexture.Sample(DefaultSampler, pIn.texturePos + float2(offset[i], 0.0) / 500) * weight[i];
		fragColor += DiffuseTexture.Sample(DefaultSampler, pIn.texturePos - float2(offset[i], 0.0) / 500) * weight[i];
	}

	//diffuse *= fragment;
	return fragColor;
}