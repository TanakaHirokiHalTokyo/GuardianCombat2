//===================================================================================================
//
//		Cube.h
//
//===================================================================================================


#ifndef _3DPOLYGON_H_
#define _3DPOLYGON_H_

#include "../Object.h"
#include <iostream>
#include <fstream>
class OBB;
//=================================================================================
//		define
//=================================================================================
#define VERTEX_NUM		(24)																	//���_��
#define INDEX_NUM		(36)																	//�C���f�b�N�X��
//=================================================================================

class Vector3;

class Cube :public Object
{
public:

	struct VERTEX3D
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};
public:
	Cube();
	~Cube()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void Draw(LPD3DXEFFECT effect, UINT pass);
	void EndDraw()override;

	OBB* GetCollision();								//�R���W�����擾
	void SetCollision(OBB* collision);			//�R���W�����ݒ�

	OBB* AddCollision();								//�R���W�����ǉ�
	float GetAttackValue();							//�U���͎擾
	bool GetHit();											//���łɓ������Ă���̂����擾
	void SetHit(bool flag);								//������������ݒ�
	void Hit();												//�����������ɌĂяo��
	void SaveParameter(const std::string filename);		//�����o�ϐ��t�@�C�������o��
	void LoadParameter(const std::string filename);		//�ϐ��t�@�C���ɏ����o�����f�[�^�ǂݍ���
private:
	const float ATTACK_VALUE = 10.0f;				//��{�U����

	LPDIRECT3DTEXTURE9			pTexture_ = nullptr;									//�e�N�X�`���|�C���^
	LPDIRECT3DVERTEXBUFFER9		pVertexBuffer_ = NULL;					//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9		pIndexBuffer_ = NULL;					//�C���f�b�N�X�o�b�t�@
	D3DMATERIAL9				mat_ = {};														//�}�e���A�����
	OBB* collision_ = nullptr;																		//�R���W����
	float attack_ = ATTACK_VALUE;															//�U����
	bool isHit = false;																					//���łɓ���������
};


#endif // !_3DPOLYGON_H_
