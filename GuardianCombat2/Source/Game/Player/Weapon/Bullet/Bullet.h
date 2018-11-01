#pragma once

//=====================================================================
//
//		Bullet�N���X
//
//=====================================================================

#include "../../../Object.h"
class Weapon;

class Bullet :public Object
{
public:
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

protected:
	float attack_ = 1.0f;			//�U����
	float rate_ = 1.0f;				//��b�ԂɑłĂ��

	Weapon* weapon_ = nullptr;			//���˂��镐��
};