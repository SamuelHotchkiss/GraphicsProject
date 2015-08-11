struct V_IN
{
	float4 posH : SV_POSITION;
	float3 uvH  : UV;
	float3 nrm  : NORMAL;
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
	float4 finalColor = baseColor;
	//finalColor.a = baseColor.b;
	//finalColor.r = baseColor.g;
	//finalColor.g = baseColor.r;
	//finalColor.b = baseColor.a;

	float3 ldir = -normalize(lightDir);
	float3 wnrm = normalize(input.nrm);


	//float3 surfacePos = input.posH.xyz;
	//float3 ptldir = normalize(ptLightPos - surfacePos);


	finalColor = saturate((clamp(dot(ldir, wnrm), 0, 1) * finalColor * lightColor) /*+ (clamp(dot(ptldir, wnrm), 0, 1) * finalColor * ptLightColor)*/ + (finalColor * ambientColor));

	return finalColor;
}