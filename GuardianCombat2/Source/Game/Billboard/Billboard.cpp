
#include "Billboard.h"
#include "../../main.h"
#include "../../DirectXRenderer.h"
#include "../../Imgui/ImguiManager.h"
#include "../../Camera/Camera.h"


Billboard::Billboard()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();


	VERTEX3D* Vertex;
	WORD* pIndex;

	//FVF�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX3D);


	//VertexBuffer�쐬
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX3D) * 4,				//���_�����̃T�C�Y
		D3DUSAGE_WRITEONLY,					//�������݂������Ȃ�
		FVF_VERTEX3D,						//FVF
		D3DPOOL_MANAGED,					//�f�o�C�X������ꂽ�Ƃ�����ɕ�������
		&this->pVertexBuffer_,					//���_�΂��ӂ����Ǘ����邽�߂̃A�h���X	�g��Ȃ��Ȃ�����Release
		NULL
	);

	//Vertex���o�^
	this->pVertexBuffer_->Lock(
		0,
		0,
		(void**)&Vertex,
		D3DLOCK_DISCARD						//Lock���Ă���͈͂��������ݐ�p�ŏ㏑��������
	);

	//���_�o�^
	Vertex[0].pos = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	Vertex[1].pos = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	Vertex[2].pos = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	Vertex[3].pos = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);

	//�@���o�^
	Vertex[0].vec =
		Vertex[1].vec =
		Vertex[2].vec =
		Vertex[3].vec = D3DXVECTOR3(0, 0, 1.0f);

	//���_�J���[�o�^
	Vertex[0].color =
		Vertex[1].color =
		Vertex[2].color =
		Vertex[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	this->pVertexBuffer_->Unlock();

	//Index�o�b�t�@�쐬
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 6,					//�C���f�b�N�X�̃T�C�Y
		D3DUSAGE_WRITEONLY,					//�������݂������Ȃ�
		D3DFMT_INDEX16,						//(index�̌^ ��WORD(unsigned short)�̂���
		D3DPOOL_MANAGED,					//�f�o�C�X������ꂽ�Ƃ�����ɕ�������
		&this->pIndexBuffer_,					//�C���f�b�N�X�΂��ӂ����Ǘ����邽�߂̃A�h���X	�g��Ȃ��Ȃ�����Release
		NULL
	);

	//Index���o�^
	this->pIndexBuffer_->Lock(
		0,
		0,
		(void**)&pIndex,
		D3DLOCK_DISCARD
	);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 0;
	pIndex[4] = 2;
	pIndex[5] = 3;

	this->pIndexBuffer_->Unlock();
}

Billboard::~Billboard()
{
	if (this->pIndexBuffer_ != nullptr)
	{
		this->pIndexBuffer_->Release();
		this->pIndexBuffer_ = nullptr;
	}
	if (this->pVertexBuffer_ != nullptr)
	{
		this->pVertexBuffer_->Release();
		this->pVertexBuffer_ = nullptr;
	}
}

void Billboard::Init()
{
	SetPosition(0,0,0);
}

void Billboard::Uninit()
{
	
}

void Billboard::Update()
{
	CAMERA_INFO camInfo = Camera::GetCameraInfo();

	this->SetView(camInfo.view);
}

void Billboard::BeginDraw()
{
	//World�s��Ɋe�s����Z�b�g
	D3DXMATRIX mtxTrans, mtxScale;

	D3DXMatrixTranslation(&mtxTrans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixScaling(&mtxScale, transform_.scale.x, transform_.scale.y, transform_.scale.z);


	world_ = this->mtxView_;
	world_ *= mtxScale;
	world_ *= mtxTrans;
}

void Billboard::Draw()
{
	if (!useShader_)
	{
		LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
		VERTEX3D* Vertex;


		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		//�A���t�@�e�X�g��TRUE�ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


		//FVF�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX3D);

		//���C�g�I�t�ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//�e�N�X�`���Z�b�g
		pDevice->SetTexture(0, this->pTexture_);
		//�e��s��̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &world_);

		//Vertex���o�^
		this->pVertexBuffer_->Lock(
			0,
			0,
			(void**)&Vertex,
			D3DLOCK_DISCARD						//Lock���Ă���͈͂��������ݐ�p�ŏ㏑��������
		);

		float u0 = (float)this->texcoord_.x / this->texSize_.x;
		float v0 = (float)this->texcoord_.y / this->texSize_.y;
		float u1 = (float)(this->texcoord_.x + this->texcoordSize_.x) / this->texSize_.x;
		float v1 = (float)(this->texcoord_.y + this->texcoordSize_.y) / this->texSize_.y;

		Vertex[0].color =
			Vertex[1].color =
			Vertex[2].color =
			Vertex[3].color = this->color_;

		Vertex[0].texcoord = D3DXVECTOR2(u0, v0);
		Vertex[1].texcoord = D3DXVECTOR2(u1, v0);
		Vertex[2].texcoord = D3DXVECTOR2(u1, v1);
		Vertex[3].texcoord = D3DXVECTOR2(u0, v1);

		this->pVertexBuffer_->Unlock();



		// GPU��VertexBuffer���p�C�v���C���łȂ� - +
		pDevice->SetStreamSource(
			0,						//�p�C�v���C���̃Z�b�g�ԍ�
			this->pVertexBuffer_,		//�o�b�t�@
			0,						//�ǂ����珑�����ނ�
			sizeof(VERTEX3D)		//�ׂ̒��_�܂łǂꂭ�炢�̒���
		);

		// �f�o�C�X�ɃC���f�b�N�X�Z�b�g
		pDevice->SetIndices(
			this->pIndexBuffer_				//�o�b�t�@
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


		//�A���t�@�e�X�g��FALSE�ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//���C�g�I���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	

}

void Billboard::Draw(LPD3DXEFFECT effect, UINT pass)
{
	if(useShader_)
	{
		effect->Begin(NULL,0);
		effect->BeginPass(pass);

		LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
		VERTEX3D* Vertex;


		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		//�A���t�@�e�X�g��TRUE�ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


		//FVF�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX3D);
		//�e��s��̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &world_);

		//���C�g�I�t�ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		effect->SetVector("Diffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		effect->SetTexture("MeshTex", pTexture_);
		effect->CommitChanges();
		

		//Vertex���o�^
		this->pVertexBuffer_->Lock(
			0,
			0,
			(void**)&Vertex,
			D3DLOCK_DISCARD						//Lock���Ă���͈͂��������ݐ�p�ŏ㏑��������
		);

		float u0 = (float)this->texcoord_.x / this->texSize_.x;
		float v0 = (float)this->texcoord_.y / this->texSize_.y;
		float u1 = (float)(this->texcoord_.x + this->texcoordSize_.x) / this->texSize_.x;
		float v1 = (float)(this->texcoord_.y + this->texcoordSize_.y) / this->texSize_.y;

		Vertex[0].color =
			Vertex[1].color =
			Vertex[2].color =
			Vertex[3].color = this->color_;

		Vertex[0].texcoord = D3DXVECTOR2(u0, v0);
		Vertex[1].texcoord = D3DXVECTOR2(u1, v0);
		Vertex[2].texcoord = D3DXVECTOR2(u1, v1);
		Vertex[3].texcoord = D3DXVECTOR2(u0, v1);

		this->pVertexBuffer_->Unlock();



		// GPU��VertexBuffer���p�C�v���C���łȂ� - +
		pDevice->SetStreamSource(
			0,						//�p�C�v���C���̃Z�b�g�ԍ�
			this->pVertexBuffer_,		//�o�b�t�@
			0,						//�ǂ����珑�����ނ�
			sizeof(VERTEX3D)		//�ׂ̒��_�܂łǂꂭ�炢�̒���
		);

		// �f�o�C�X�ɃC���f�b�N�X�Z�b�g
		pDevice->SetIndices(
			this->pIndexBuffer_				//�o�b�t�@
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


		//�A���t�@�e�X�g��FALSE�ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//���C�g�I���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		effect->EndPass();
		effect->End();
	}
}

void Billboard::EndDraw()
{
}

void Billboard::SetView(const D3DXMATRIX & view)
{
	if (stand_)
	{
		D3DXMatrixIdentity(&this->mtxView_);
		this->mtxView_ = view;
		D3DXMatrixTranspose(&mtxView_, &mtxView_);

		mtxView_._14 = 0.0f;				//41�̕��s�ړ������������Ă���̂�0��
		mtxView_._24 = 0.0f;				//42�̕��s�ړ������������Ă���̂�0��
		mtxView_._34 = 0.0f;				//43�̕��s�ړ������������Ă���̂�0��

		mtxView_._12 = 0.0f;
		mtxView_._21 = 0.0f;

		mtxView_._23 = 0.0f;
		mtxView_._32 = 0.0f;
	}
	else
	{
		D3DXMatrixIdentity(&this->mtxView_);
		this->mtxView_ = view;
		D3DXMatrixTranspose(&mtxView_, &mtxView_);

		mtxView_._14 = 0.0f;				//41�̕��s�ړ������������Ă���̂�0��
		mtxView_._24 = 0.0f;				//42�̕��s�ړ������������Ă���̂�0��
		mtxView_._34 = 0.0f;				//43�̕��s�ړ������������Ă���̂�0��
	}
}

D3DXMATRIX Billboard::GetView()
{
	return mtxView_;
}
void Billboard::SetTexture(TextureManager::TextureList texture)
{
	TextureManager::TexInfo textureInfo = TextureManager::GetTexture(texture);
	pTexture_ = textureInfo.pTex;
	texSize_ = D3DXVECTOR2(textureInfo.width, textureInfo.height);
	texcoordSize_ = D3DXVECTOR2(textureInfo.texcoordX, textureInfo.texcoordY);
}

void Billboard::SetStand(bool stand)
{
	stand_ = stand;
}
