struct P_IN
{
	float4 posH : SV_POSITION;
	float2 uv  : UV;
	float3 nrm  : NORMAL;
	float4 posLT : LT_POSITION;
};

texture2D baseTexture : register(t0);

SamplerState filter : register(s0);

float4 main(P_IN input) : SV_TARGET
{
	float4 postColor = baseTexture.Sample(filter, input.uv);

	if (postColor.a < 0.1f)
		discard;
	float postR = 1.0f - postColor.r;
	float postG = 1.0f - postColor.g;
	float postB = 1.0f - postColor.b;
	
	postColor = float4(postR, postG, postB, 1.0f);

	return postColor;
}