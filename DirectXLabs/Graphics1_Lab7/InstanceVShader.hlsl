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
	float4x4 worldMatrices[4];
}

cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
}


V_OUT main(V_IN input, uint instanceID : SV_InstanceID)
{
	V_OUT output = (V_OUT)0;

	float4 localH = float4(input.posL, 1.0f);
	float3 nrm = input.nrm;

	//localH.xyz.x += instanceID;

	localH = mul(localH, worldMatrices[instanceID]);
	nrm = mul(nrm, worldMatrices[instanceID]);
	output.posLT = localH;
	// TODO: Move into view space, then projection space
	localH = mul(localH, viewMatrix);
	//nrm = mul(nrm, viewMatrix);
	localH = mul(localH, projMatrix);
	//nrm = mul(nrm, projMatrix);

	output.posH = localH;
	//output.colH = float4(1.0f,0.0f,0.0f,1.0f);
	output.uv = input.uv;
	output.nrm = nrm;

	return output;
}