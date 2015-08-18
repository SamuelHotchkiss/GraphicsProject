TextureCube qube : register(t0);

SamplerState filter[2] : register(s0);

struct V_IN
{
	float4 pos : SV_POSITION;
	float3 loPos : LOCAL_POSITION;
};

float4 main(V_IN input) : SV_TARGET
{
	float4 skyColor = qube.Sample(filter[0], input.loPos);
	return skyColor;
}