#pragma pack_matrix(row_major)

struct V_IN
{
	float4 pos : POSITION;
	float4 col : COLOR;
};

struct V_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

cbuffer WORLD : register(b0)
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
	V_OUT output;

	float4 pos = input.pos;

	pos = mul(pos, worldMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projMatrix);

	output.pos = pos;
	output.col = input.col;

	return output;
}