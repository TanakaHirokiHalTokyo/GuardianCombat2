#pragma once

//=====================================================================
//
//		Bullet�N���X
//
//=====================================================================

#include "../../../Object.h"
class Weapon;
class Billboard;
class Sphere;
class DebugSphere;

class Bullet :public Object
{
public:
	struct BulletInfo
	{
		Billboard* bullet_ = nullptr;													//�e�̃r���{�[�h
		Sphere* collision_ = nullptr;												//�R���W�������
		DebugSphere* debug_ = nullptr;										//�R���W�����\��
		float speed_ = 0.1f;																//�e�̃X�s�[�h
		bool use_ = false;																//�g�p���Ă���̂�
		size_t count_ = 0;																//�\���J�E���^
		D3DXVECTOR3 vector_ = D3DXVECTOR3(0, 0, 0);				//�e�̔��ł����x�N�g��								
	};

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	//�U���͂�ݒ�E�擾
	void SetAttack(float value) { attack_ = value; }
	float GetAttack() { return attack_; }
	
	//��b�ԂɑłĂ��
	void SetRate(float value) { rate_ = value; }
	float GetRate() { return rate_; }

	//���˂��镐���ݒ�E�擾
	void SetWeapon(Weapon* weapon) { weapon_ = weapon; }
	Weapon* GetWeapon() { return weapon_; }

	void SetShooting(D3DXVECTOR3 pos, D3DXVECTOR3 vec, float speed);

	bool GetShooting()
	{
		return bulletInfo_.use_;
	}

	void Destroy();

protected:
	BulletInfo bulletInfo_ = {};										//�e�̏��
	float attack_ = 1.0f;													//�U����
	float rate_ = 1.0f;														//��b�ԂɑłĂ��
	Weapon* weapon_ = nullptr;									//���˂��镐��
	bool use_ = false;													//�g�p���Ă���̂�
};