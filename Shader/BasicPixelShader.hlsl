#include "BasicShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v��

float4 BasicPS(BasicType input) : SV_TARGET{
	//return float4((float2(0,1) + input.pos.xy) * 0.5f,1,1);
	//return float4(input.uv,1,1);
	//return float4(tex.Sample(smp,input.uv));
	return float4(0,0,0,1);
}