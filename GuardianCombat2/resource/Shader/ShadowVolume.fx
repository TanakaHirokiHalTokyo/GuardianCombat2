//==============================================================
// �萔
float4x4 World;
float4x4 WView;
float4x4 WVProj;
float4x4 Proj;
float3 Light;
float4 Ambient;
float4 Diffuse;
texture MeshTex;
float4 VolumeColor;
float NearZ;
float FarZ;

//==============================================================
// �e�N�X�`���T���v��
sampler MeshSmp=sampler_state {
	Texture=<MeshTex>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};

//==============================================================
// �V�F�[�_�i��{�j

// ���_�V�F�[�_
void BasicVS (
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0,
	out float4 Pos : POSITION,
	out float4 Col : COLOR0,
	out float2 MeshUV : TEXCOORD0
) {
	// ���_���W�A�e�N�X�`�����W
	Pos=mul(pos, WVProj);
	MeshUV=mesh_uv;

	// ���_�F
	float3 w_normal=normalize(mul(normal, (float3x3)World));
	Col=max(0, dot(w_normal, -Light))+Ambient;
}

// �s�N�Z���V�F�[�_
float4 BasicPS(
	float4 Col : COLOR0,
	float2 MeshUV : TEXCOORD0
) : COLOR {
	return tex2D(MeshSmp, MeshUV)*Col*Diffuse;
}

//==============================================================
// �V�F�[�_�i�V���h�E�{�����[���j

// ���_�V�F�[�_
void VolumeVS(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	out float4 Pos : POSITION
) {
	// ���_���W�Ɩ@�������[���h�r���[�ϊ�
	float4 wv_pos=mul(pos, WView);
	float3 wv_normal=mul(normal, (float3x3)WView);

	// �����ɑ΂��ė������̒��_�����������Ɉړ�������
	if (dot(wv_normal, -Light)<0) {
		if (Light.z>0) {
			wv_pos.xyz+=Light*abs(FarZ-wv_pos.z)/Light.z;
		} else {
			wv_pos.xyz+=Light*abs(NearZ-wv_pos.z)/Light.z;
		}
		Pos=mul(wv_pos, Proj);
	} else {
		Pos=mul(pos, WVProj);
	}
}

// �s�N�Z���V�F�[�_
float4 VolumePS() : COLOR {
	return VolumeColor;
}

//==============================================================
// �V�F�[�_�i�e�j

float4 ShadowPS(
	float4 Col : COLOR0
) : COLOR {
	return Col;
}

//==============================================================
// �e�N�j�b�N
technique Basic {
	pass P0 {
		VertexShader=compile vs_3_0 BasicVS();
		PixelShader=compile ps_3_0 BasicPS();
	}
}
technique ZOnly {
	pass P0 {
		VertexShader=compile vs_3_0 BasicVS();
		PixelShader=compile ps_3_0 BasicPS();
		ColorWriteEnable=0;
	}
}
technique Volume {
	pass P0 {
		VertexShader=compile vs_3_0 VolumeVS();
		CullMode=CCW;
		ColorWriteEnable=0;
		ZWriteEnable=FALSE;
		ZFunc=LESS;
		StencilEnable=TRUE;
		StencilPass=INCR;
	}
	pass P1 {
		VertexShader=compile vs_3_0 VolumeVS();
		CullMode=CW;
		StencilPass=DECR;
	}
}
technique Shadow {
	pass P0 {
		PixelShader=compile ps_3_0 ShadowPS();
		ZEnable=FALSE;
		StencilEnable=TRUE;
		StencilFunc=LESS;
	}
}
technique ShowVolume {
	pass P0 {
		VertexShader=compile vs_3_0 VolumeVS();
		PixelShader=compile ps_3_0 VolumePS();
		CullMode=CCW;
		ZWriteEnable=FALSE;
		ZFunc=LESS;
		StencilEnable=TRUE;
		StencilPass=INCR;
	}
	pass P1 {
		VertexShader=compile vs_3_0 VolumeVS();
		PixelShader=compile ps_3_0 VolumePS();
		CullMode=CW;
		StencilPass=DECR;
	}
}

