struct V_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float3 nrm : NORMAL;
};

cbuffer DIRLIGHT : register(b0)
{
	float3 lightDir;
	float4 lightColor;
}
cbuffer AMBIENT : register(b1)
{
	float4 ambientColor;
}

float4 main(V_IN input) : SV_TARGET
{
	float3 ldir = -normalize(lightDir);
	float3 wnrm = normalize(input.nrm);
	//float4 ambientColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
	//float lightRatio = clamp(dot(-lightDir, input.nrm), 0.0f, 1.0f);
	float4 litColor = saturate((clamp(dot(ldir,wnrm),0,1) * input.col * lightColor) + (input.col * ambientColor));
	//litColor.w = 1.0f;
	return litColor;
}