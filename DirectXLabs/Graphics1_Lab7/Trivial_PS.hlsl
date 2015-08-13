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
	float4 spotLightColor;
	float spotNada;
}


float4 main( V_IN input ) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.uvH.xy);

	//float4 attResult;
	float4 dirDiffResult;
	float4 ptDiffResult;
	float4 spotDiffResult;

	float4 specResult;

	float3 wnrm = normalize(input.nrm);
	//DIRECTION LIGHT
	float3 ldir = -normalize(lightDir);

	float lRatio = saturate(dot(ldir, wnrm));

	//POINT LIGHT
	float3 pldir = normalize(ptLightPos - input.posLT.xyz);
	float plRatio = saturate(dot(pldir, wnrm));
	
	float theLength = length(ptLightPos - input.posLT.xyz);
	float attRatio = theLength / 3;
	float theClamp = saturate(attRatio);
	float attenuation = 1.0f - theClamp;//clamp((length(ptLightPos - input.posH.xyz) / 10000), 0, 1);
	attenuation *= attenuation;
	//SPOTLIGHT
	/*
		LIGHTDIR = NORMALIZE( LIGHTPOS – SURFACEPOS ) )
		SURFACERATIO = CLAMP( DOT( -LIGHTDIR, CONEDIR ) )
		SPOTFACTOR = ( SURFACERATIO > CONERATIO ) ? 1 : 0
		LIGHTRATIO = CLAMP( DOT( LIGHTDIR, SURFACENORMAL ) )
		RESULT = SPOTFACTOR * LIGHTRATIO * LIGHTCOLOR * SURFACECOLOR
	*/
	
	float3 spldir = normalize(spotLightPos - input.posLT.xyz);
	float splRatio = saturate(dot(spldir, wnrm));

	theLength = length(spotLightPos - input.posLT.xyz);
	float surfaceRatio = saturate(dot(-spldir, coneDir));
	float spotFactor = (surfaceRatio > coneRatio) ? 1 : 0;
	float spotAttenuation = 1.0f - saturate((coneRatio - surfaceRatio));
	spotAttenuation *= spotAttenuation;

	dirDiffResult = (baseColor * lightColor * lRatio) + (baseColor * ambientColor);
	ptDiffResult = (baseColor * ptLightColor * plRatio) + (baseColor * ambientColor);
	ptDiffResult *= attenuation;
	spotDiffResult = (baseColor * spotLightColor * splRatio * spotFactor) + (baseColor * ambientColor);
	spotDiffResult *= spotAttenuation;

	float4 finalColor = saturate(dirDiffResult + ptDiffResult + spotDiffResult);
	//finalColor.a = baseColor.b;
	//finalColor.r = baseColor.g;
	//finalColor.g = baseColor.r;
	//finalColor.b = baseColor.a;

	/*finalColor = saturate((clamp(dot(ldir, wnrm), 0, 1) * finalColor * lightColor) + (finalColor * ambientColor));*/

	return finalColor;
}