#pragma pack_matrix(row_major)

struct V_IN
{
	float3 posL : POSITION;
	float2 uv  : UV;
	float3 nrm  : NORMAL;
};

struct V_OUT
{
	float4 posH : SV_POSITION;
	float2 uv  : UV;
	float3 nrm  : NORMAL;
	float4 posLT : LT_POSITION;
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


V_OUT main(V_IN input)
{
	V_OUT output = (V_OUT)0;

	float4 localH = float4(input.posL, 1.0f);
	float3 nrm = input.nrm;

	localH = mul(localH, worldMatrix);
	nrm = mul(nrm, worldMatrix);
	output.posLT = localH;
	localH = mul(localH, viewMatrix);
	localH = mul(localH, projMatrix);

	output.posH = localH;
	
	output.uv = input.uv;
	output.nrm = nrm;

	return output;
}