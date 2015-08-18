#pragma pack_matrix(row_major)

struct V_IN
{
	float3 pos : POSITION;
	float2 uv : UV;
	float3 nrm : NORMAL;
};

struct V_OUT
{
	float4 pos : SV_POSITION;
	float3 loPos : LOCAL_POSITION;
};

cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;
}

cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
}

V_OUT main( V_IN input)
{
	V_OUT output = (V_OUT)0;
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, worldMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projMatrix);

	output.pos = pos;
	output.loPos = input.pos;

	return output;
}