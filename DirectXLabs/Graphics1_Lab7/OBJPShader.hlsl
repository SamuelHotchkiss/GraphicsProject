struct V_IN
{
	float4 pos : SV_POSITION;
	float2 uv  : UV;
	float3 nrm : NORMAL;
};

float4 main(V_IN input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}