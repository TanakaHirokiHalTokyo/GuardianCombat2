#pragma once

//========================================================
//
//		Weapon�N���X
//
//========================================================

#include "../../Object.h"	

class Player;
class XModel;
class WeaponState;
class Bullet;
class Vector3;

class Weapon:public Object
{
public:
	virtual ~Weapon() {};
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	//�e�ƂȂ�v���C���[��ݒ�E�擾
	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer() { return player_; }

	//���΍��W�擾
	D3DXVECTOR3 GetRelativePosition() { return relativePosition_; }

	//�e�̎擾
	Bullet* GetBullet() { return bullet_; }

protected:
	//�U���͐ݒ�E�擾
	void SetAttack(float Attack) { attack_ = Attack; }
	float GetAttack() { return attack_; }
	
	//���΍��W�ݒ�
	void SetRelativePosition(D3DXVECTOR3 pos) { relativePosition_ = pos; }
	void SetRelativePosition(float x, float y, float z) { relativePosition_ = D3DXVECTOR3(x, y, z); }

	//�e�̐ݒ�
	void SetBullet(Bullet* bullet) { bullet_ = bullet; }
	
protected:
	void ShotBullet(int BulletNum);

protected:
	float attack_ = 1.0f;																						//�U����
	Player* player_ = nullptr;																			//�v���C���[���B�A�^�b�`����ۂȂǂɎg�p
	XModel* model_ = nullptr;																			//�e�̃��f��
	WeaponState* relative_ = nullptr;																//���Ί֌W����
	D3DXVECTOR3 relativePosition_ = D3DXVECTOR3(0,0,0);						//���΍��W
	Bullet* bullet_ = nullptr;																				//�e�̏��
	float fireRate_ = 10.0f;																				//1�b�Ԃɉ������Ă�̂��B
	size_t rateCount_ = 0;																					//�A�ˎ��J�E���^
};