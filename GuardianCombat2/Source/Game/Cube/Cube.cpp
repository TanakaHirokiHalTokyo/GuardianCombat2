
#include "../../main.h"
#include "../../DirectXRenderer.h"
#include "../../Texture/Texture.h"
#include "Cube.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ShadowMapShader.h"
#include "../Shader/ToonShader.h"
#include "../../Imgui/ImguiManager.h"
#include "../../Vector3/Vector3.h"

const D3DXVECTOR3 X_VECTOR = D3DXVECTOR3(1.0f,0.0f,0.0f);
const D3DXVECTOR3 Y_VECTOR = D3DXVECTOR3(0.0f,1.0f,0.0f);
const D3DXVECTOR3 Z_VECTOR = D3DXVECTOR3(0.0f,0.0f,1.0f);

Cube::Cube()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	VERTEX3D* pV;
	LPWORD	pIndex;

	useShader_ = true;
	HRESULT hr;

	pTexture_ = TextureManager::GetTexture(TextureManager::Tex_Turret).pTex;

	//�}�e���A��
	ZeroMemory(&mat_, sizeof(mat_));
	mat_.Diffuse.r = 1.0f;
	mat_.Diffuse.g = 1.0f;
	mat_.Diffuse.b = 1.0f;
	mat_.Diffuse.a = 1.0f;

	mat_.Ambient.r = 0.2f;
	mat_.Ambient.g = 0.2f;
	mat_.Ambient.b = 0.2f;
	mat_.Ambient.a = 1.0f;

	//���_�̍쐬
	VERTEX3D v[] = {
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),	D3DXVECTOR3(0,0,1.0f)	,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),	D3DXVECTOR3(0,0,1.0f),		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),	D3DXVECTOR3(0,0,1.0f),		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),	D3DXVECTOR3(0,0,1.0f),		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,1.0f) },
		{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),	D3DXVECTOR3(0,0,-1.0f),		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),	D3DXVECTOR3(0,0,-1.0f),		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DXVECTOR3(0,0,-1.0f),		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),	D3DXVECTOR3(0,0,-1.0f),		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,1.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),	D3DXVECTOR3(1.0f,0,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),	D3DXVECTOR3(1.0f,0,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),	D3DXVECTOR3(1.0f,0,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),	D3DXVECTOR3(1.0f,0,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,1.0f) },
		{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),	D3DXVECTOR3(-1.0f,0,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),	D3DXVECTOR3(-1.0f,0,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,1.0f) },
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),	D3DXVECTOR3(-1.0f,0,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),	D3DXVECTOR3(-1.0f,0,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),	D3DXVECTOR3(0,1.0f,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),	D3DXVECTOR3(0,1.0f,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),	D3DXVECTOR3(0,1.0f,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),	D3DXVECTOR3(0,1.0f,0) ,		D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,1.0f) },
		{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),	D3DXVECTOR3(0,-1.0f,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),	D3DXVECTOR3(0,-1.0f,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),	D3DXVECTOR3(0,-1.0f,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),	D3DXVECTOR3(0,-1.0f,0) ,	D3DCOLOR_RGBA(255,255,255,255),	D3DXVECTOR2(1.0f,1.0f) },
	};
	WORD Index[] = {
		0,1,2,
		2,1,3,
		4,5,6,
		5,7,6,
		8,9,10,
		10,9,11,
		12,13,14,
		14,13,15,
		16,17,18,
		18,17,19,
		20,21,22,
		21,23,22
	};

	//VertexBuffer�쐬
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX3D) * VERTEX_NUM,		//���_�����̃T�C�Y
		D3DUSAGE_WRITEONLY,					//�������݂������Ȃ�
		FVF_VERTEX3D,						//FVF
		D3DPOOL_MANAGED,					//�f�o�C�X������ꂽ�Ƃ�����ɕ�������
		&pVertexBuffer_,					//���_�΂��ӂ����Ǘ����邽�߂̃A�h���X	�g��Ȃ��Ȃ�����Release
		NULL
	);

	pVertexBuffer_->Lock(
		0,
		0,
		(void**)&pV,
		D3DLOCK_DISCARD						//�������񂾂�J���Ȃ��@���x�d��	Unlock������f�[�^�͎�����
	);
	memcpy(pV, v, sizeof(v));

	pVertexBuffer_->Unlock();

	//Index�o�b�t�@�쐬
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * INDEX_NUM,			//�C���f�b�N�X�̃T�C�Y
		D3DUSAGE_WRITEONLY,					//�������݂������Ȃ�
		D3DFMT_INDEX16,						//(index�̌^ ��WORD(unsigned short)�̂���
		D3DPOOL_MANAGED,					//�f�o�C�X������ꂽ�Ƃ�����ɕ�������
		&pIndexBuffer_,					//�C���f�b�N�X�΂��ӂ����Ǘ����邽�߂̃A�h���X	�g��Ȃ��Ȃ�����Release
		NULL
	);

	pIndexBuffer_->Lock(
		0,
		0,
		(void**)&pIndex,
		D3DLOCK_DISCARD
	);


	memcpy(pIndex, Index, sizeof(Index));

	pIndexBuffer_->Unlock();

	//�R���W�����쐬
	collision_ = AddCollision();
	collision_->enable_ = false;
	collision_->m_Pos = GetPosition();
	collision_->m_fLength[0] = 1.0f;
	collision_->m_fLength[1] = 1.0f;
	collision_->m_fLength[2] = 1.0f;
	collision_->m_NormaDirect[0] = X_VECTOR;
	collision_->m_NormaDirect[1] = Y_VECTOR;
	collision_->m_NormaDirect[2] = Z_VECTOR;
	collision_->object_ = this;
}
Cube::~Cube()
{
	//���_�o�b�t�@���
	if (pVertexBuffer_)
	{
		pVertexBuffer_->Release();
		pVertexBuffer_ = NULL;
	}
	//�C���f�b�N�X�o�b�t�@���
	if (pIndexBuffer_)
	{
		pIndexBuffer_->Release();
		pIndexBuffer_ = NULL;
	}
}
void Cube::Init()
{
	this->transform_.pos = D3DXVECTOR3(0, 0, 0);
	this->transform_.rotate = D3DXVECTOR3(0, 0, 0);
	this->transform_.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	isHit = false;
}
void Cube::Uninit()
{
	
}
void Cube::Update()
{
	
}
void Cube::BeginDraw()
{
	if (GetVisible())
	{
		//�R���W�����̑傫���X�V
		collision_->m_fLength[0] = GetScale().x;
		collision_->m_fLength[1] = GetScale().y;
		collision_->m_fLength[2] = GetScale().z;

		D3DXMATRIX mtxTrans, mtxScale, mtxRotateX, mtxRotateY, mtxRotateZ;
		
		D3DXMatrixIdentity(&mtxRotateX);
		D3DXMatrixIdentity(&mtxRotateY);
		D3DXMatrixIdentity(&mtxRotateZ);

		D3DXMatrixTranslation(&mtxTrans, Object::transform_.pos.x, Object::transform_.pos.y, Object::transform_.pos.z);
		D3DXMatrixScaling(&mtxScale, Object::transform_.scale.x, Object::transform_.scale.y, Object::transform_.scale.z);
		D3DXMatrixRotationX(&mtxRotateX, D3DXToRadian(Object::transform_.rotate.x));
		D3DXMatrixRotationY(&mtxRotateY, D3DXToRadian(Object::transform_.rotate.y));
		D3DXMatrixRotationZ(&mtxRotateZ, D3DXToRadian(Object::transform_.rotate.z));
		D3DXMatrixIdentity(&world_);

		world_ = mtxScale; 
		world_ *= mtxRotateX;
		world_ *= mtxRotateY;
		world_ *= mtxRotateZ;
		world_ *= mtxTrans;

		//======================================================================
		//		OBB�x�N�g����]
		//======================================================================
		D3DXVECTOR3 VEC;
		D3DXMATRIX rotate;
		D3DXMatrixRotationX(&rotate,D3DXToRadian(transform_.rotate.x));
		D3DXVec3TransformNormal(&VEC, &X_VECTOR, &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[0] = VEC;
		D3DXVec3TransformNormal(&VEC, &Y_VECTOR, &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[1] = VEC;
		D3DXVec3TransformNormal(&VEC, &Z_VECTOR, &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[2] = VEC;

		D3DXMatrixIdentity(&rotate);
		D3DXMatrixRotationY(&rotate, D3DXToRadian(transform_.rotate.y));
		D3DXVec3TransformNormal(&VEC, &collision_->m_NormaDirect[0], &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[0] = VEC;
		D3DXVec3TransformNormal(&VEC, &collision_->m_NormaDirect[1], &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[1] = VEC;
		D3DXVec3TransformNormal(&VEC, &collision_->m_NormaDirect[2], &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[2] = VEC;

		D3DXMatrixIdentity(&rotate);
		D3DXMatrixRotationZ(&rotate, D3DXToRadian(transform_.rotate.z));
		D3DXVec3TransformNormal(&VEC, &collision_->m_NormaDirect[0], &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[0] = VEC;
		D3DXVec3TransformNormal(&VEC, &collision_->m_NormaDirect[1], &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[1] = VEC;
		D3DXVec3TransformNormal(&VEC, &collision_->m_NormaDirect[2], &rotate);
		D3DXVec3Normalize(&VEC, &VEC);
		collision_->m_NormaDirect[2] = VEC;

	

		if (useShader_)
		{
			LPD3DXEFFECT effect;
			ShadowMapShader::SetInfo(world_);
			effect = ShadowMapShader::GetEffect();
			effect->SetTechnique("Shadow");
			Draw(effect, 0);
		}
	}
}
void Cube::Draw()
{
	if (!GetUseShader())
	{
		if (GetVisible())
		{
			LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

			//FVF�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX3D);
			//���C�g�I���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//�e��s��̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &world_);
			pDevice->SetTexture(0, pTexture_);

			//�O���[�o���A���r�G���g
			pDevice->SetRenderState(D3DRS_AMBIENT,
				D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));


			// GPU��VertexBuffer���p�C�v���C���łȂ� - +
			pDevice->SetStreamSource(
				0,						//�p�C�v���C���̃Z�b�g�ԍ�
				pVertexBuffer_,		//�o�b�t�@
				0,						//�ǂ����珑�����ނ�
				sizeof(VERTEX3D)		//�ׂ̒��_�܂łǂꂭ�炢�̒���
			);

			pDevice->SetIndices(
				pIndexBuffer_				//�o�b�t�@
			);

			//������o�b�t�@�g�������Ȃ��Ƃ��������Ƃ��o�b�t�@��NULL�ɂ���Ύg��Ȃ��Ȃ�B

			pDevice->SetMaterial(&mat_);

			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				VERTEX_NUM,			//���_��
				0,
				12			//�v���~�e�B�u��
			);
		}
	}
	else
	{
		LPD3DXEFFECT effect;

		ShadowMapShader::SetInfo(world_);
		effect = ShadowMapShader::GetEffect();
		effect->SetTechnique("Scene");
		Draw(effect, 0);

		//�g�D�[���V�F�[�_�[�`��
		effect = ToonShader::GetEffect();
		ToonShader::SetInfo(world_);
		effect->SetTechnique("ToonPaintInk");
		Draw(effect, 0);
	}
}

void Cube::Draw(LPD3DXEFFECT effect, UINT pass)
{
	

	if (GetUseShader())
	{
		if (GetVisible())
		{
			effect->Begin(NULL, 0);
			effect->BeginPass(pass);

			LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

			//FVF�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX3D);
			//���C�g�I���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//�e��s��̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &world_);

			D3DXVECTOR4 diffuse = D3DXVECTOR4(mat_.Diffuse.r, mat_.Diffuse.g, mat_.Diffuse.b, mat_.Diffuse.a);
			effect->SetVector("Diffuse", &diffuse);
			D3DXVECTOR4 ambient = D3DXVECTOR4(mat_.Ambient.r, mat_.Ambient.g, mat_.Ambient.b, mat_.Ambient.a);
			effect->SetVector("Ambient", &ambient);
			effect->SetTexture("MeshTex", pTexture_);
			effect->CommitChanges();

			// GPU��VertexBuffer���p�C�v���C���łȂ� - +
			pDevice->SetStreamSource(
				0,						//�p�C�v���C���̃Z�b�g�ԍ�
				pVertexBuffer_,		//�o�b�t�@
				0,						//�ǂ����珑�����ނ�
				sizeof(VERTEX3D)		//�ׂ̒��_�܂łǂꂭ�炢�̒���
			);

			pDevice->SetIndices(
				pIndexBuffer_				//�o�b�t�@
			);

			//������o�b�t�@�g�������Ȃ��Ƃ��������Ƃ��o�b�t�@��NULL�ɂ���Ύg��Ȃ��Ȃ�B

			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				VERTEX_NUM,			//���_��
				0,
				12			//�v���~�e�B�u��
			);
			effect->EndPass();
			effect->End();
		}
	}
}

void Cube::EndDraw()
{
}

OBB* Cube::GetCollision()
{
	return collision_;
}

void Cube::SetCollision(OBB * collision)
{
	collision_ = collision;
}

OBB * Cube::AddCollision()
{
	OBB* collision = new OBB();
	enemycubeCollisions_.emplace_back(collision);
	return collision;
}

float Cube::GetAttackValue()
{
	return attack_;
}

bool Cube::GetHit()
{
	return isHit;
}

void Cube::Hit()
{
	isHit = true;
}

