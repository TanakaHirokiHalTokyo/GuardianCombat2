//==============================================================
// �萔
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
// �e�N�X�`���T���v��
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
// �V�F�[�_�i�y�C���g�j

// ���_�V�F�[�_
void ToonPaintVS(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0,
	out float4 Pos : POSITION,
	out float2 MeshUV : TEXCOORD0,
	out float2 ToonUV : TEXCOORD1
) {
	// ���_���W�A�e�N�X�`�����W
	Pos = mul(pos, WVProj);
	MeshUV = mesh_uv;

	// �g�D�[���e�N�X�`�����W
	float3 w_normal = normalize(mul(normal, (float3x3)World));
	ToonUV.x = max(0, dot(w_normal, -Light)) + Ambient;
	ToonUV.y = 0.5;
}

// �s�N�Z���V�F�[�_
float4 ToonPaintPS(
	float2 MeshUV : TEXCOORD0,
	float2 ToonUV : TEXCOORD1
) : COLOR{
	return
	tex2D(MeshSmp, MeshUV)*
	tex2D(ToonSmp, ToonUV)*Diffuse;
}

//==============================================================
// �V�F�[�_�i�C���N�j

// ���_�V�F�[�_
void ToonInkVS(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	out float4 Pos : POSITION
) {
	// ���_���W
	float4 wvp_pos = mul(pos, WVProj);
	float4 wvp_normal = float4(mul(normal, (float3x3)WVProj), 0);
	Pos = wvp_pos + normalize(wvp_normal)*InkSize*wvp_pos.w;
}

// �s�N�Z���V�F�[�_
float4 ToonInkPS() : COLOR{
	return InkColor;
}

//==============================================================
// �V�F�[�_�i�~�b�N�X�j

// ���_�V�F�[�_
void ToonMixedVS(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0,
	out float4 Pos : POSITION,
	out float2 MeshUV : TEXCOORD0,
	out float2 ToonUV : TEXCOORD1
) {
	// ���_���W�A�e�N�X�`�����W
	Pos = mul(pos, WVProj);
	MeshUV = mesh_uv;

	// �g�D�[���e�N�X�`�����W�i�y�C���g�j
	float3 w_normal = normalize(mul(normal, (float3x3)World));
	ToonUV.x = max(0, dot(w_normal, -Light)) + Ambient;

	// �g�D�[���e�N�X�`�����W�i�C���N�j
	float3 wv_pos = normalize(mul(pos, WView));
	float3 wv_normal = normalize(mul(normal, (float3x3)WView));
	ToonUV.y = dot(wv_pos, wv_normal)*0.5 + 0.5;
}

// �s�N�Z���V�F�[�_
float4 ToonMixedPS(
	float2 MeshUV : TEXCOORD0,
	float2 ToonUV : TEXCOORD1
) : COLOR{
	return
	tex2D(MeshSmp, MeshUV)*
	tex2D(ToonSmp, ToonUV)*Diffuse;
}

//==============================================================
// �e�N�j�b�N
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
