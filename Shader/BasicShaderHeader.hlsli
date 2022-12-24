struct Input {
	float4 pos:POSITION;
	float4 svpos:SV_POSITION;
};

struct Output {
	float4 pos:POSITION;
	float4 svpos:SV_POSITION;
	float2 uv : TEXCOORD;
};