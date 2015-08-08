struct V_IN
{
	float4 posH : SV_POSITION;
	float3 uvH  : UV;
	float3 nrm  : NORMAL;
};

texture2D baseTexture : register(t0);

SamplerState filters[2] : register(s0);

float4 main( V_IN input ) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.uvH.xy);
	float4 finalColor = baseColor;
	finalColor.a = baseColor.b;
	finalColor.r = baseColor.g;
	finalColor.g = baseColor.r;
	finalColor.b = baseColor.a;

	return finalColor;
}