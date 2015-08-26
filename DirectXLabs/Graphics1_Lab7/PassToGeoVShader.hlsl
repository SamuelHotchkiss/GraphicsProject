struct V_IN
{
	float3 posL : POSITION;
	float2 uv  : UV;
	float3 nrm  : NORMAL;
};

V_IN main( V_IN input )
{
	return input;
}