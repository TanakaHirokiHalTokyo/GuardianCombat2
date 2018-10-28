//-------------------------------------------------
//	�O���[�o���ϐ�
//-------------------------------------------------

float4x4 mWVP;		//���[���h����[���r���[�[���v���W�F�N�V�����܂ł̍��W�ϊ�
texture MeshTex;


//---------------------------------------
//		Sampler
//--------------------------------------
sampler smp = sampler_state {
	texture = <MeshTex>;
};

//-------------------------------------
//		�\����
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
//		���_�V�F�[�_
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
//		�s�N�Z���V�F�[�_
//------------------------------------------
float4 ps_main(VS_OUT In) : COLOR0
{
	In.color.rgb = 0.0f;
	In.color.r = 0.0f;
	In.color.a = 1.0f;
	return In.color;
}
//-------------------------------------------
//		�e�N�j�b�N
//-------------------------------------------
technique Tech {
	pass p0 {
		CullMode = CW;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
};