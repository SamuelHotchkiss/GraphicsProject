#pragma pack_matrix(row_major)

struct GSInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 nrm : NORMAL;
	float4 posLT : LT_POSITION;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 nrm : NORMAL;
	float4 posLT : LT_POSITION;
};

cbuffer OBJECT : register (b0)
{
	float4x4 worldMatrix;
}
cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
}

[maxvertexcount(3)]
void main(
	triangle GSInput input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element = input[i];
		output.Append(element);
	}
}