#pragma pack_matrix(row_major)

struct V_IN
{
	float4 pos : POSITION;
	float4 col : COLOR;
	float3 nrm : NORMAL;
};

struct V_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float3 nrm : NORMAL;
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
	float3 nrm = input.nrm;

	pos = mul(pos, worldMatrix);
	nrm = mul(nrm, worldMatrix);

	pos = mul(pos, viewMatrix);
	nrm = mul(nrm, viewMatrix);

	pos = mul(pos, projMatrix);
	nrm = mul(nrm, projMatrix);


	output.pos = pos;
	output.col = input.col;
	output.nrm = nrm;

	return output;
}