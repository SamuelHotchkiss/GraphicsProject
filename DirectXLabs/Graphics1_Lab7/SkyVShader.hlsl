struct V_IN
{
	float3 pos : POSITION;
};

struct V_OUT
{
	float4 pos : SV_POSITION;
};

V_OUT main( V_IN input) : SV_POSITION
{
	V_OUT output = (V_OUT)0;
	output.pos = float4(input.pos, 1.0f);
	return output;
}