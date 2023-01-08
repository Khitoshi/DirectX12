#include "BasicShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラ

float4 BasicPS(BasicType input) : SV_TARGET{
	//return float4((float2(0,1) + input.pos.xy) * 0.5f,1,1);
	//return float4(input.uv,1,1);
	//return float4(tex.Sample(smp,input.uv));
	return float4(0,0,0,1);
}