//==============================================================
// �萔
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
// �e�N�X�`���T���v��
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
// �V�F�[�_�i�V���h�E�}�b�v�̕`��j

// ���_�V�F�[�_
void ShadowVS (
	float4 pos : POSITION,
	float3 normal : NORMAL,
	out float4 Pos : POSITION,
	out float2 Depth : TEXCOORD0
) {
	// ���_���W�����C�g�ɑ΂��Ďˉe�ϊ�
	Pos=mul(mul(pos, World), LightVProj);
	
	// �[�x
	Depth.xy=Pos.zw;
}

// �s�N�Z���V�F�[�_
float4 ShadowPS(
	float2 Depth : TEXCOORD0
) : COLOR {
	return Depth.x/Depth.y;
}

//==============================================================
// �V�F�[�_�i�V�[���̕`��j

// ���_�V�F�[�_
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
	// ���_���W�A�e�N�X�`�����W
	Pos=mul(pos, WVProj);
	MeshUV=mesh_uv;

	// ���_���W�Ɩ@�������[���h�ϊ�
	WPos=mul(pos, World);
	WNormal=normalize(mul(normal, (float3x3)World));
	
	// ���_���W�����C�g�ɑ΂��Ďˉe�ϊ�
	LVPPos=mul(WPos, LightVProj);
}

// �s�N�Z���V�F�[�_
float4 ScenePS(
	float2 MeshUV : TEXCOORD0,
	float4 WPos : TEXCOORD1, 
	float3 WNormal : TEXCOORD2, 
	float4 LVPPos : TEXCOORD3
) : COLOR {

	// �s�N�Z���̐F
	float4 color=Ambient;

	// �s�N�Z�������C�g�͈͓̔��ɂ���ꍇ
	float2 xy=LVPPos.xy/LVPPos.w;
	if (abs(xy.x)<=1 && abs(xy.y)<=1 && LVPPos.z>=0) {

		// ���C�g�ɑ΂��Ďˉe�ϊ��������_���W��
		// �V���h�E�}�b�v��̃e�N�X�`�����W�ɂ���
		float2 uv=xy*0.5f+0.5f;
		uv.y=1-uv.y;

		// �אڂ���4�e�N�Z���Ɋւ���
		// ���C�g�������邩�ǂ����𔻒肷��
		float z=LVPPos.z/LVPPos.w-Margin;
		float d=1.0f/TEX_SIZE;
		float t0, t1, t2, t3;
		t0=(tex2D(ShadowSmp, uv).r<z)?0:1;
		t1=(tex2D(ShadowSmp, uv+float2(d, 0)).r<z)?0:1;
		t2=(tex2D(ShadowSmp, uv+float2(0, d)).r<z)?0:1;
		t3=(tex2D(ShadowSmp, uv+float2(d, d)).r<z)?0:1;

		// ���茋�ʂ���`��Ԃ���
		float2 f=frac(uv*TEX_SIZE);
		float light_power=lerp(
			lerp(t0, t1, f.x), 
			lerp(t2, t3, f.x), f.y);

		// �s�N�Z���Ƀ��C�g�̐F��������
		float3 light=normalize(float3(WPos-LightWPos));
		color+=max(0, dot(WNormal, -light))*light_power*LightColor;
	}

	// ���b�V���e�N�X�`���Ƃ̍���
	return float4((tex2D(MeshSmp, MeshUV)*color*Diffuse).rgb, Diffuse.a);
}

// �e�̂Ȃ��V�[��
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

// �V���h�E�}�b�v�̕\��
float4 ShowShadowPS(
	float2 MeshUV : TEXCOORD0
) : COLOR {
	float z=tex2D(ShadowSmp, MeshUV).r;
	return float4(z*Diffuse.rgb, Diffuse.a);
}

//==============================================================
// �e�N�j�b�N
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
