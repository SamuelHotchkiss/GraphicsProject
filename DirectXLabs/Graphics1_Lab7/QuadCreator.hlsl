#pragma pack_matrix(row_major)

struct V_IN
{
	float3 posL : POSITION;
	float2 uv  : UV;
	float3 nrm  : NORMAL;
};

struct GSOutput
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

[maxvertexcount(6)]
void main(
	point V_IN input[1],
	inout TriangleStream< GSOutput > output
	)
{
	GSOutput elements[6];

	for (uint i = 0; i < 6; i++)
	{
		elements[i].posH = float4(input[0].posL, 1.0f);
		elements[i].nrm = input[0].nrm;
	}

	elements[0].posH += float4(-1.0f, 1.0f, 0.0f, 0.0f);
	elements[0].uv = float2(0.0f, 0.0f);
	elements[1].posH += float4(1.0f, 1.0f, 0.0f, 0.0f);
	elements[1].uv = float2(1.0f, 0.0f);
	elements[2].posH += float4(1.0f, -1.0f, 0.0f, 0.0f);
	elements[2].uv = float2(1.0f, 1.0f);
	elements[3] = elements[0];
	elements[4] = elements[2];
	elements[5].posH += float4(-1.0f, -1.0f, 0.0f, 0.0f);
	elements[5].uv = float2(0.0f, 1.0f);


	for (uint i = 0; i < 6; i++)
	{
		elements[i].posH = mul(elements[i].posH, worldMatrix);
		elements[i].nrm = mul(elements[i].nrm, worldMatrix);
		elements[i].posLT = elements[i].posH;

		elements[i].posH = mul(elements[i].posH, viewMatrix);
		elements[i].posH = mul(elements[i].posH, projMatrix);

		output.Append(elements[i]);
	}
}