//==============================================================
// 定数
float4x4 World;
float4x4 WView;
float4x4 WVProj;
float3 LightWPos;
float4x4 LightVProj;
float4 LightColor;
float4 Ambient;
float4 Diffuse;
texture MeshTex;
texture ShadowTex;
float Margin;

#define TEX_SIZE 4096

//==============================================================
// テクスチャサンプラ
sampler MeshSmp=sampler_state {
	Texture=<MeshTex>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};
sampler ShadowSmp=sampler_state {
	Texture=<ShadowTex>;
	MipFilter=POINT;
	MinFilter=POINT;
	MagFilter=POINT;
	AddressU=CLAMP;
	AddressV=CLAMP;
};

//==============================================================
// シェーダ（シャドウマップの描画）

// 頂点シェーダ
void ShadowVS (
	float4 pos : POSITION,
	float3 normal : NORMAL,
	out float4 Pos : POSITION,
	out float2 Depth : TEXCOORD0
) {
	// 頂点座標をライトに対して射影変換
	Pos=mul(mul(pos, World), LightVProj);
	
	// 深度
	Depth.xy=Pos.zw;
}

// ピクセルシェーダ
float4 ShadowPS(
	float2 Depth : TEXCOORD0
) : COLOR {
	return Depth.x/Depth.y;
}

//==============================================================
// シェーダ（シーンの描画）

// 頂点シェーダ
void SceneVS (
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0, 
	out float4 Pos : POSITION, 
	out float2 MeshUV : TEXCOORD0, 
	out float4 WPos : TEXCOORD1, 
	out float3 WNormal : TEXCOORD2, 
	out float4 LVPPos : TEXCOORD3
) {
	// 頂点座標、テクスチャ座標
	Pos=mul(pos, WVProj);
	MeshUV=mesh_uv;

	// 頂点座標と法線をワールド変換
	WPos=mul(pos, World);
	WNormal=normalize(mul(normal, (float3x3)World));
	
	// 頂点座標をライトに対して射影変換
	LVPPos=mul(WPos, LightVProj);
}

// ピクセルシェーダ
float4 ScenePS(
	float2 MeshUV : TEXCOORD0,
	float4 WPos : TEXCOORD1, 
	float3 WNormal : TEXCOORD2, 
	float4 LVPPos : TEXCOORD3
) : COLOR {

	// ピクセルの色
	float4 color=Ambient;

	// ピクセルがライトの範囲内にある場合
	float2 xy=LVPPos.xy/LVPPos.w;
	if (abs(xy.x)<=1 && abs(xy.y)<=1 && LVPPos.z>=0) {

		// ライトに対して射影変換した頂点座標を
		// シャドウマップ上のテクスチャ座標にする
		float2 uv=xy*0.5f+0.5f;
		uv.y=1-uv.y;

		// 隣接する4テクセルに関して
		// ライトが当たるかどうかを判定する
		float z=LVPPos.z/LVPPos.w-Margin;
		float d=1.0f/TEX_SIZE;
		float t0, t1, t2, t3;
		t0=(tex2D(ShadowSmp, uv).r<z)?0:1;
		t1=(tex2D(ShadowSmp, uv+float2(d, 0)).r<z)?0:1;
		t2=(tex2D(ShadowSmp, uv+float2(0, d)).r<z)?0:1;
		t3=(tex2D(ShadowSmp, uv+float2(d, d)).r<z)?0:1;

		// 判定結果を線形補間する
		float2 f=frac(uv*TEX_SIZE);
		float light_power=lerp(
			lerp(t0, t1, f.x), 
			lerp(t2, t3, f.x), f.y);

		// ピクセルにライトの色を加える
		float3 light=normalize(float3(WPos-LightWPos));
		color+=max(0, dot(WNormal, -light))*light_power*LightColor;
	}

	// メッシュテクスチャとの合成
	return float4((tex2D(MeshSmp, MeshUV)*color*Diffuse).rgb, Diffuse.a);
}

// 影のないシーン
float4 SceneNoShadowPS(
	float2 MeshUV : TEXCOORD0,
	float4 WPos : TEXCOORD1, 
	float3 WNormal : TEXCOORD2, 
	float4 LVPPos : TEXCOORD3
) : COLOR {
	float3 light=normalize(float3(WPos-LightWPos));
	float4 color=Ambient;
	float2 xy=LVPPos.xy/LVPPos.w;
	if (abs(xy.x)<1 && abs(xy.y)<1 && LVPPos.z>0) {
		color+=max(0, dot(WNormal, -light));
	}
	return float4((tex2D(MeshSmp, MeshUV)*color*Diffuse).rgb, Diffuse.a);
}

// シャドウマップの表示
float4 ShowShadowPS(
	float2 MeshUV : TEXCOORD0
) : COLOR {
	float z=tex2D(ShadowSmp, MeshUV).r;
	return float4(z*Diffuse.rgb, Diffuse.a);
}

//==============================================================
// テクニック
technique Shadow {
	pass P0 {
		VertexShader=compile vs_3_0 ShadowVS();
		PixelShader=compile ps_3_0 ShadowPS();
	}
}
technique Scene {
	pass P0 {
		VertexShader=compile vs_3_0 SceneVS();
		PixelShader=compile ps_3_0 ScenePS();
	}
}
technique SceneNoShadow {
	pass P0 {
		VertexShader=compile vs_3_0 SceneVS();
		PixelShader=compile ps_3_0 SceneNoShadowPS();
	}
}
technique ShowShadow {
	pass P0 {
		PixelShader=compile ps_3_0 ShowShadowPS();
	}
}
