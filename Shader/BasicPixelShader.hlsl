#include "BasicShaderHeader.hlsli"

float4 BasicPS(Output input) : SV_TARGET{
	//return float4((float2(0,1) + input.pos.xy) * 0.5f,1,1);
	return float4(input.uv,1,1);
}