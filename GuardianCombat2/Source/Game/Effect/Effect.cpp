#include "Effect.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "../../DirectXRenderer.h"
#include "../../Camera/Camera.h"

LPDIRECT3DTEXTURE9 EffectManager::texture_ = nullptr;
LPDIRECT3DVERTEXBUFFER9 EffectManager::vertexBuffer_ = nullptr;
LPDIRECT3DINDEXBUFFER9 EffectManager::indexBuffer_ = nullptr;
EffectManager::VERTEX3D* EffectManager::vertex_ = nullptr;

void EffectManager::Init()
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	WORD* pIndex;
	HRESULT hr;

	//�G�t�F�N�g�e�N�X�`���ǂݍ���
	const char filename[256] = "resource/texture/Effect_100.png";
	hr = D3DXCreateTextureFromFileA(pDevice, filename,&texture_);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,"�G�t�F�N�g�ǂݍ��ݎ��s","�x��",MB_OK | MB_ICONHAND);
	}

	//VertexBuffer�쐬
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&vertexBuffer_,
		NULL
	);

	//Vertex���o�^
	vertexBuffer_->Lock(
		0,
		0,
		(void**)&vertex_,
		D3DLOCK_DISCARD
	);
	vertex_[0] = { D3DXVECTOR3(-0.5f,0.5f,0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(0.0f,0.0f) };
	vertex_[1] = { D3DXVECTOR3(0.5f, 0.5f, 0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(1.0f,0.0f) };
	vertex_[2] = { D3DXVECTOR3(0.5f, -0.5f, 0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(1.0f,1.0f) };
	vertex_[3] = { D3DXVECTOR3(-0.5f, -0.5f, 0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(0.0f,1.0f) };

	vertexBuffer_->Unlock();

	//Index�o�b�t�@�쐬
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&indexBuffer_,
		NULL
	);

	//Index���o�^
	indexBuffer_->Lock(0,0,(void**)&pIndex,D3DLOCK_DISCARD);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 0;
	pIndex[4] = 2;
	pIndex[5] = 3;

	indexBuffer_->Unlock();


}

void EffectManager::Uninit()
{
	if (texture_)
	{
		texture_->Release();
		texture_ = nullptr;
	}
	if (indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
}

LPDIRECT3DTEXTURE9 EffectManager::GetTexture()
{
	return texture_;
}

LPDIRECT3DVERTEXBUFFER9& EffectManager::GetVertexBuffer()
{
	return vertexBuffer_;
}

LPDIRECT3DINDEXBUFFER9& EffectManager::GetIndexBuffer()
{
	return indexBuffer_;
}

EffectManager::VERTEX3D & EffectManager::GetVertex()
{
	return *vertex_;
}

AdditionEffect::AdditionEffect()
{
	limit_ = 150;
	for (int i = 0; i < EFFECTNUM; i++)
	{
		color_[i].r = 1.0f;
		color_[i].g = 1.0f;
		color_[i].b = 1.0f;
		color_[i].a = 1.0f;
		transform_[i].scale = D3DXVECTOR3(1, 1, 1);
		transform_[i].pos = D3DXVECTOR3(0,0,0);
		count_[i] = 0;
		use_[i] = false;
	}
}

AdditionEffect::~AdditionEffect()
{
	
}

void AdditionEffect::Init()
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		count_[i] = 0;
		use_[i] = false;
		color_[i].a = 1.0f;
		transform_[i].pos = D3DXVECTOR3(0,0,0);
	}
}

void AdditionEffect::Uninit()
{
	limit_ = 150;
	for (int i = 0; i < EFFECTNUM; i++)
	{
		count_[i] = 0;
		use_[i] = false;
		color_[i].a = 0.0f;
		transform_[i].pos = D3DXVECTOR3(0, 0, 0);
	}
}

void AdditionEffect::Update()
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		if (use_[i])
		{
			//�����v�Z
			float ratio = (float)(count_[i] / limit_);

			//���l�X�V
			color_[i].a = 1.0f - ratio;

			if (ratio >= 1.0f)
			{
				use_[i] = false;
			}

			//�J�E���^�X�V
			count_[i]++;
		}
	}
}

void AdditionEffect::Draw()
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		if (use_[i])
		{
			//�f�o�C�X�擾
			LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

			//vertex���擾
			EffectManager::VERTEX3D* vertex = &EffectManager::GetVertex();

			EffectManager::GetVertexBuffer()->Lock(
				0, 0, (void**)&vertex, D3DLOCK_DISCARD);

			//�J���[�ăZ�b�g
			vertex[0].color = (D3DCOLOR)color_[i];
			vertex[1].color = (D3DCOLOR)color_[i];
			vertex[2].color = (D3DCOLOR)color_[i];
			vertex[3].color = (D3DCOLOR)color_[i];

			EffectManager::GetVertexBuffer()->Unlock();

			//�J�������擾
			CAMERA_INFO cam_info = Camera::GetCameraInfo();
			D3DXMATRIX view = cam_info.view;

			//�t�s��ɕϊ�
			D3DXMatrixTranspose(&view, &view);

			view._14 = 0.0f;
			view._24 = 0.0f;
			view._34 = 0.0f;

			D3DXMATRIX mtxTrans, mtxScale, mtxWorld;

			//�ړ��E�g��s��쐬
			D3DXMatrixTranslation(&mtxTrans, transform_[i].pos.x, transform_[i].pos.y, transform_[i].pos.z);
			D3DXMatrixScaling(&mtxScale, transform_[i].scale.x, transform_[i].scale.y, transform_[i].scale.z);

			mtxWorld = view;
			mtxWorld *= mtxScale;
			mtxWorld *= mtxTrans;

			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			//FVF�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX3D);
			//���C�g�I�t�ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			//�e�N�X�`���Z�b�g
			pDevice->SetTexture(0, EffectManager::GetTexture());
			//�e��s��ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// GPU��VertexBuffer���p�C�v���C���łȂ� - +
			pDevice->SetStreamSource(
				0,										//�p�C�v���C���̃Z�b�g�ԍ�
				EffectManager::GetVertexBuffer(),		//�o�b�t�@
				0,										//�ǂ����珑�����ނ�
				sizeof(EffectManager::VERTEX3D)			//�ׂ̒��_�܂łǂꂭ�炢�̒���
			);

			// �f�o�C�X�ɃC���f�b�N�X�Z�b�g
			pDevice->SetIndices(
				EffectManager::GetIndexBuffer()			//�o�b�t�@
			);

			//������o�b�t�@�g�������Ȃ��Ƃ��������Ƃ��o�b�t�@��NULL�ɂ���Ύg��Ȃ��Ȃ�B

			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				4,			//���_��
				0,
				2			//�v���~�e�B�u��
			);


			//���C�g�I���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			////�ʏ�̃��u�����h�i�����������j�ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		}
	}

	
}

void AdditionEffect::CreateEffect(D3DXVECTOR3 pos)
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		if (!use_[i])
		{
			use_[i] = true;
			transform_[i].pos = pos;
			break;
		}
	}
}
void AdditionEffect::SetLimit(int limit)
{
	limit_ = limit;
}

void AdditionEffect::SetColor(float r,float g,float b)
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		color_[i].r = r;
		color_[i].g = g;
		color_[i].b = b;
	}
}

void AdditionEffect::SetScale(float size)
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		transform_[i].scale = D3DXVECTOR3(size, size, size);
	}
}
