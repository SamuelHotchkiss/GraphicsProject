#pragma pack_matrix(row_major)

struct V_IN
{
	float3 pos : POSITION;
	float2 uv  : UV;
	float3 nrm : NORMAL;
};

struct V_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : UV;
	float3 nrm : NORMAL;
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

V_OUT main( V_IN input )
{
	V_OUT output = (V_OUT)0;

	float4 changePos = float4(input.pos, 1.0f);
	changePos = mul(changePos, worldMatrix);
	changePos = mul(changePos, viewMatrix);
	changePos = mul(changePos, projMatrix);

	output.pos = changePos;
	output.uv = input.uv;
	output.nrm = input.nrm;

	return output;
}