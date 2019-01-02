#pragma once

#include "../Object.h"
#include "../../Texture/Texture.h"

class Billboard : public Object
{
public :
	enum Billboard_Tex
	{
		BilTex_Turret,					
		BilTex_Difficulity,
		BilTex_SimpleBullet,
		BilTex_Max
	};

	struct VERTEX3D									//���_�\����
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	Billboard();																		//�R���X�g���N�^
	~Billboard()override;														//�f�X�g���N�^
	void Init()override;															//����������
	void Uninit()override;														//�I������
	void Update()override;													//�X�V����
	void BeginDraw()override;												//���O�`�揈��
	void Draw()override;														//�`�揈��
	void Draw(LPD3DXEFFECT effect,UINT pass);				//�`�揈���V�F�[�_�[�g�p��
	void EndDraw()override;													//�`��I������
	void SetView(const D3DXMATRIX& view);						//�r���[�s��ݒ�
	D3DXMATRIX GetView();												//�r���[�s��擾
	void SetTexture(TextureManager::TextureList texture);		//�e�N�X�`���擾
	void SetColor(D3DCOLOR color) { this->color_ = color; }	//�J���[�ݒ�
	void SetTexcoord(int tcx, int tcy) { this->texcoord_.x = (float)tcx; this->texcoord_.y = (float)tcy; }					//�e�N�X�`�����W�ݒ�
	void SetTexcoordSize(float tcw, float tch) { this->texcoordSize_.x = tcw; this->texcoordSize_.y = tch; }		//�e�N�X�`��1�A�j���[�V�����̑傫���ݒ�
	void SetStand(bool stand);												//Pitch��������̂�
private:
	bool stand_ = false;																						//Pitch����t���O�@
	LPDIRECT3DTEXTURE9 pTexture_ = NULL;												//�e�N�X�`���|�C���^
	D3DXMATRIX mtxView_ = {};																		//�r���[�s��
	D3DCOLOR color_ = D3DCOLOR_RGBA(255,255,255,255);						//�F���
	D3DXVECTOR2 texcoord_ = D3DXVECTOR2(0, 0);									//�e�N�X�`�����W
	D3DXVECTOR2 texSize_ = D3DXVECTOR2(1, 1);										//�e�N�X�`���T�C�Y
	D3DXVECTOR2 texcoordSize_ = D3DXVECTOR2(1, 1);								//�e�N�X�`���P�A�j���[�V�����̑傫��
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer_ = NULL;							//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer_ = NULL;								//�C���f�b�N�X�o�b�t�@
};