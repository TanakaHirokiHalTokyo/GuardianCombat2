//-------------------------------------------------
//	グローバル変数
//-------------------------------------------------

float4x4 mWVP;		//ワールドからー＞ビューー＞プロジェクションまでの座標変換
texture MeshTex;


//---------------------------------------
//		Sampler
//--------------------------------------
sampler smp = sampler_state {
	texture = <MeshTex>;
};

//-------------------------------------
//		構造体
//-------------------------------------
struct VS_IN
{
	float4 pos:POSITION;
	float4 color: COLOR;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD0;
};
struct VS_OUT
{
	float4 pos:POSITION;
	float4 color: COLOR;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD0;
};
//-------------------------------------------
//		頂点シェーダ
//-------------------------------------------
VS_OUT vs_main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	In.pos += float4(In.normal * 0.05f,0);

	Out.pos = mul(In.pos,mWVP);
	Out.uv = In.uv;
	Out.normal = In.normal;
	Out.color = In.color;

	return Out;
}
//-------------------------------------------
//		ピクセルシェーダ
//------------------------------------------
float4 ps_main(VS_OUT In) : COLOR0
{
	In.color.rgb = 0.0f;
	In.color.r = 0.0f;
	In.color.a = 1.0f;
	return In.color;
}
//-------------------------------------------
//		テクニック
//-------------------------------------------
technique Tech {
	pass p0 {
		CullMode = CW;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
};