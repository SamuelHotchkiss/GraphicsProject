#pragma pack_matrix(row_major)

struct V_IN
{
	/*float4 posL : POSITION;
	float4 colL : COLOR;*/
	//float4 uvL : UV;

	float3 posL : POSITION;
	float3 uvw  : UVW;
	float3 nrm  : NORMAL;
};

struct V_OUT
{
	float4 posH : SV_POSITION;
	float3 uvH  : UV;
	float3 nrm  : NORMAL;
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
	//float4 localH = input.posL;

	localH = mul(localH, worldMatrix);
	// TODO: Move into view space, then projection space
	localH = mul(localH, viewMatrix);
	localH = mul(localH, projMatrix);

	output.posH = localH;
	//output.colH = float4(1.0f,0.0f,0.0f,1.0f);
	output.uvH = input.uvw;

	return output;
}