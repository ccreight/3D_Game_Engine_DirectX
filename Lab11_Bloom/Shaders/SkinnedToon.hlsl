#include "Constants.hlsl"

#define MAX_SKELETON_BONES 80
cbuffer SkinConstants : register(b3)
{
    float4x4 c_skinMatrix[MAX_SKELETON_BONES];
};

struct VIn
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    uint4 boneIndex : BONE0;
    float4 boneWeight : WEIGHT0;
    float2 uv : TEXCOORD0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

VOut VS(VIn vIn)
{
    VOut output;

    float4 inPos = float4(vIn.position, 1.0);
    float4 pos = mul(inPos, c_skinMatrix[vIn.boneIndex.x]) * vIn.boneWeight.x
        + mul(inPos, c_skinMatrix[vIn.boneIndex.y]) * vIn.boneWeight.y
        + mul(inPos, c_skinMatrix[vIn.boneIndex.z]) * vIn.boneWeight.z
        + mul(inPos, c_skinMatrix[vIn.boneIndex.w]) * vIn.boneWeight.w;
    float4 worldPos = mul(pos, c_modelToWorld);
    output.position = mul(worldPos, c_viewProj);
    float4 inNorm = float4(vIn.normal, 0.0);
    float4 norm = mul(inNorm, c_skinMatrix[vIn.boneIndex.x]) * vIn.boneWeight.x
        + mul(inNorm, c_skinMatrix[vIn.boneIndex.y]) * vIn.boneWeight.y
        + mul(inNorm, c_skinMatrix[vIn.boneIndex.z]) * vIn.boneWeight.z
        + mul(inNorm, c_skinMatrix[vIn.boneIndex.w]) * vIn.boneWeight.w;
    output.normal = mul(norm, c_modelToWorld).xyz;
    output.uv = vIn.uv;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
	float4 diffuse = DiffuseTexture.Sample(DefaultSampler, pIn.uv);

	// do the lighting
	 float3 lightColor = c_dirLight.color;

	float3 dist = pIn.position - c_cameraPosition;
	float dotprod = dot(pIn.normal, normalize(c_dirLight.dir));
	dotprod = clamp(dotprod, 0, 1);

	if (dotprod < .3) {
		dotprod = .15;
	}
	else if (dotprod < .6) {
		dotprod = .45;
	}
	else {
		dotprod = .8;
	}

	lightColor *= dotprod;
	lightColor += c_ambient;

	float4 finalColor = diffuse * float4(lightColor, 1.0);
	return finalColor;
}
