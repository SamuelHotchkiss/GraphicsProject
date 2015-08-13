struct V_IN
{
	float4 posH : SV_POSITION;
	float3 uvH  : UV;
	float3 nrm  : NORMAL;
	float4 posLT : LT_POSITION;
};

texture2D baseTexture : register(t0);

SamplerState filters[2] : register(s0);

cbuffer DIR_LIGHT : register(b0)
{
	float3 lightDir;
	float4 lightColor;
}

cbuffer AMBIENT : register(b1)
{
	float4 ambientColor;
}

cbuffer POINT_LIGHT : register (b2)
{
	float3 ptLightPos;
	float4 ptLightColor;
}


float4 main( V_IN input ) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.uvH.xy);

	//float4 attResult;
	float4 dirDiffResult;
	float4 ptDiffResult;
	float4 sptDiffResult;

	float4 specResult;

	float3 wnrm = normalize(input.nrm);
	//DIRECTION LIGHT
	float3 ldir = -normalize(lightDir);

	float lRatio = clamp(dot(ldir, wnrm), 0, 1);

	//POINT LIGHT
	float3 pldir = normalize(ptLightPos - input.posLT.xyz);
	float plRatio = clamp(dot(pldir, wnrm), 0, 1);
	
	float theLength = length(ptLightPos - input.posLT.xyz);
	float attRatio = theLength / 3;
	float theClamp = clamp(attRatio, 0, 1);
	float attenuation = 1.0f - theClamp;//clamp((length(ptLightPos - input.posH.xyz) / 10000), 0, 1);
	//attenuation *= attenuation;
	//SPOTLIGHT

	dirDiffResult = (baseColor * lightColor * lRatio) + (baseColor * ambientColor);
	ptDiffResult = (baseColor * ptLightColor * plRatio) + (baseColor * ambientColor);
	ptDiffResult *= attenuation;

	float4 finalColor = saturate(dirDiffResult + ptDiffResult);
	//finalColor.a = baseColor.b;
	//finalColor.r = baseColor.g;
	//finalColor.g = baseColor.r;
	//finalColor.b = baseColor.a;

	/*finalColor = saturate((clamp(dot(ldir, wnrm), 0, 1) * finalColor * lightColor) + (finalColor * ambientColor));*/

	return finalColor;
}