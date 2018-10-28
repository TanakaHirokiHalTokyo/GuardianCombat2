//==============================================================
// 定数
float4x4 World;
float4x4 WView;
float4x4 WVProj;
float4 Diffuse;
float4 Ambient;
float3 Light;
texture MeshTex;
texture ToonTex;
float4 InkColor;
float InkSize;

//==============================================================
// テクスチャサンプラ
sampler MeshSmp = sampler_state {
	Texture = <MeshTex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler ToonSmp = sampler_state {
	Texture = <ToonTex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
};

//==============================================================
// シェーダ（ペイント）

// 頂点シェーダ
void ToonPaintVS(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0,
	out float4 Pos : POSITION,
	out float2 MeshUV : TEXCOORD0,
	out float2 ToonUV : TEXCOORD1
) {
	// 頂点座標、テクスチャ座標
	Pos = mul(pos, WVProj);
	MeshUV = mesh_uv;

	// トゥーンテクスチャ座標
	float3 w_normal = normalize(mul(normal, (float3x3)World));
	ToonUV.x = max(0, dot(w_normal, -Light)) + Ambient;
	ToonUV.y = 0.5;
}

// ピクセルシェーダ
float4 ToonPaintPS(
	float2 MeshUV : TEXCOORD0,
	float2 ToonUV : TEXCOORD1
) : COLOR{
	return
	tex2D(MeshSmp, MeshUV)*
	tex2D(ToonSmp, ToonUV)*Diffuse;
}

//==============================================================
// シェーダ（インク）

// 頂点シェーダ
void ToonInkVS(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	out float4 Pos : POSITION
) {
	// 頂点座標
	float4 wvp_pos = mul(pos, WVProj);
	float4 wvp_normal = float4(mul(normal, (float3x3)WVProj), 0);
	Pos = wvp_pos + normalize(wvp_normal)*InkSize*wvp_pos.w;
}

// ピクセルシェーダ
float4 ToonInkPS() : COLOR{
	return InkColor;
}

//==============================================================
// シェーダ（ミックス）

// 頂点シェーダ
void ToonMixedVS(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0,
	out float4 Pos : POSITION,
	out float2 MeshUV : TEXCOORD0,
	out float2 ToonUV : TEXCOORD1
) {
	// 頂点座標、テクスチャ座標
	Pos = mul(pos, WVProj);
	MeshUV = mesh_uv;

	// トゥーンテクスチャ座標（ペイント）
	float3 w_normal = normalize(mul(normal, (float3x3)World));
	ToonUV.x = max(0, dot(w_normal, -Light)) + Ambient;

	// トゥーンテクスチャ座標（インク）
	float3 wv_pos = normalize(mul(pos, WView));
	float3 wv_normal = normalize(mul(normal, (float3x3)WView));
	ToonUV.y = dot(wv_pos, wv_normal)*0.5 + 0.5;
}

// ピクセルシェーダ
float4 ToonMixedPS(
	float2 MeshUV : TEXCOORD0,
	float2 ToonUV : TEXCOORD1
) : COLOR{
	return
	tex2D(MeshSmp, MeshUV)*
	tex2D(ToonSmp, ToonUV)*Diffuse;
}

//==============================================================
// テクニック
technique ToonPaintInk {
	pass P0 {
		CullMode = CCW;
		VertexShader = compile vs_3_0 ToonPaintVS();
		PixelShader = compile ps_3_0 ToonPaintPS();
	}
	pass P1 {
		CullMode = CW;
		VertexShader = compile vs_3_0 ToonInkVS();
		PixelShader = compile ps_3_0 ToonInkPS();
	}
}
technique ToonMixed {
	pass P0 {
		VertexShader = compile vs_3_0 ToonMixedVS();
		PixelShader = compile ps_3_0 ToonMixedPS();
	}
}
