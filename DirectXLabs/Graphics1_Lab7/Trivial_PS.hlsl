struct V_IN
{
	float4 posH : SV_POSITION;
	float2 uv  : UV;
	float3 nrm  : NORMAL;
	float4 posLT : LT_POSITION;
};

Texture2D baseTexture : register(t0);

SamplerState filters[2] : register(s0);

cbuffer DIR_LIGHT : register(b0)
{
	float3 lightDir;
	float dirNada;
	float4 lightColor;
}

cbuffer AMBIENT : register(b1)
{
	float4 ambientColor;
}

cbuffer POINT_LIGHT : register (b2)
{
	float3 ptLightPos;
	float ptNada;
	float4 ptLightColor;
}

cbuffer SPOT_LIGHT : register (b3)
{
	float3 spotLightPos;
	float coneRatio;
	float3 coneDir;
	float spotNada;
	float4 spotLightColor;
}


float4 main(V_IN input) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.uv.xy);

	if (baseColor.a < 0.1f)
		discard;

	float4 dirDiffResult;
	float4 ptDiffResult;
	float4 spotDiffResult;

	float4 dirSpecResult;
	float4 ptSpecResult;
	float4 spotSpecResult;

	float3 wnrm = normalize(input.nrm);
		//DIRECTIONAL LIGHT ////////////////////////////////////////////////////////////////////////////////
		float3 ldir = -normalize(lightDir);
		float lRatio = saturate(dot(ldir, wnrm));
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//POINT LIGHT //////////////////////////////////////////////////////////////////////////////////////
	float3 pldir = normalize(ptLightPos - input.posLT.xyz);
		float plRatio = saturate(dot(pldir, wnrm));

	float theLength = length(ptLightPos - input.posLT.xyz);
	float attRatio = theLength / 3;
	float theClamp = saturate(attRatio);
	float attenuation = 1.0f - theClamp;
	attenuation *= attenuation;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//SPOTLIGHT ////////////////////////////////////////////////////////////////////////////////////////
	float3 spldir = normalize(spotLightPos - input.posLT.xyz);
		float splRatio = saturate(dot(spldir, wnrm));

	theLength = length(spotLightPos - input.posLT.xyz);
	float surfaceRatio = saturate(dot(-spldir, coneDir));
	float spotFactor = (surfaceRatio > coneRatio) ? 1 : 0;
	float spotAttenuation = 1.0f - saturate((coneRatio - surfaceRatio));
	spotAttenuation *= spotAttenuation;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//BRINGING IT ALL TOGETHER
	dirDiffResult = (baseColor * lightColor * lRatio);// +(baseColor * ambientColor);
	//dirSpecResult = (lightColor * dirIntensity);

	ptDiffResult = (baseColor * ptLightColor * plRatio);// +(baseColor * ambientColor);
	ptDiffResult *= attenuation;

	spotDiffResult = (baseColor * spotLightColor * splRatio * spotFactor);// +(baseColor * ambientColor);
	spotDiffResult *= spotAttenuation;

	float4 finalColor = saturate(dirDiffResult + ptDiffResult + spotDiffResult + (baseColor * ambientColor));

	return finalColor;
}