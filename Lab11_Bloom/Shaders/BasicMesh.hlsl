#include "Constants.hlsl"

struct VIn
{
	float3 position : POSITION0;
	float4 color : COLOR0;
	float2 texturePos : TEXCOORD0;
	float3 normal : NORMAL0;

};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 texturePos : TEXCOORD0;
	float4 normal : NORMAL0;
	float4 worldPos : POSITION0;

};

VOut VS(VIn vIn)
{
	VOut output;

	// Model space into world space
	output.position = mul(float4(vIn.position, 1.0), c_modelToWorld);

	// View projection
	output.position = mul(output.position, c_viewProj);

	output.color = vIn.color;
	output.texturePos = vIn.texturePos;

	// Transformations
	output.normal = mul(float4(vIn.normal, 0.0f), c_modelToWorld);
	output.worldPos = mul(float4(vIn.position, 1.0f), c_modelToWorld);

	return output;
}

float4 PS(VOut pIn) : SV_TARGET
{	
	// Initialize result to the ambient color
	float3 result = c_ambient; 
	float3 n = normalize(pIn.normal.xyz);

	// Loop over all lights
	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		if (c_pointLight[i].isEnabled) {

			// Falloff for lighting
			float dist = distance(pIn.worldPos.xyz, c_pointLight[i].position); //should some be zero?

			if (dist > 0.0f)
			{
				dist = smoothstep(c_pointLight[i].outerRadius, c_pointLight[i].innerRadius, dist);

				// Summation variable calculations
				float3 di = c_pointLight[i].diffuseColor;
				float3 li = normalize(c_pointLight[i].position.xyz - pIn.worldPos.xyz);
				//li = normalize(max(li, 0.0f));
				float3 si = c_pointLight[i].specularColor;
				float3 ri = reflect(-li, n);
				float3 vi = normalize(c_cameraPosition - pIn.worldPos.xyz);
				float yi = c_pointLight[i].specularPower;

				// Actual summation
				result += di * dist * dot(n, li) + dist * si * pow(max(0.0f, dot(ri, vi)), yi);
			}
		}

	}
	
	return float4(result, 1.0f) * (DiffuseTexture.Sample(DefaultSampler, pIn.texturePos) * pIn.color);

}
