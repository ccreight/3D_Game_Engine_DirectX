// We want to use row major matrices
#pragma pack_matrix(row_major)

#define MAX_POINT_LIGHTS 8
#define MAX_SKELETON_BONES 80

struct PointLightData
{
	float3 diffuseColor;
	//float pad0;
	float3 specularColor;
	//float pad1;
	float3 position;
	//float pad2;
	float specularPower;
	float innerRadius;
	float outerRadius;
	bool isEnabled;

};
cbuffer LightingConstants : register(b2)
{
	float3 c_ambient;
	PointLightData c_pointLight[MAX_POINT_LIGHTS];
};

cbuffer PerCameraConstants : register(b0)
{
    float4x4 c_viewProj;
    float3 c_cameraPosition;
};

cbuffer PerObjectConstants : register(b1)
{
    float4x4 c_modelToWorld;
};

cbuffer SkinnedConstants : register(b3)
{
	float4x4 c_skinMatrix[MAX_SKELETON_BONES];
};

SamplerState DefaultSampler : register(s0);
Texture2D DiffuseTexture : register(t0);
Texture2D TextureSlot : register(t1);