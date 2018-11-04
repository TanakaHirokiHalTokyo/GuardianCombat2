#pragma once

//================================================================
//
//		Shotgun�̒e�N���X
//
//================================================================

#include "Bullet.h"
class CEffekseer;
class Sphere;
class DebugSphere;

const unsigned int BulletNum = 6;		//�U�e����e�̐�

class Bullet_Shotgun :public Bullet
{
public:
	Bullet_Shotgun();
	~Bullet_Shotgun();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
	
	void SetShooting();
	bool GetShooting();


	                  
private:
	CEffekseer* bullet_[BulletNum];										//�U�e�̃G�t�F�N�g
	D3DXVECTOR3 bulletVector_[BulletNum] = { D3DXVECTOR3(0,0,0) };		//�e�̔��ˌ���
	bool shot_ = false;													//���˒���
	int count_ = 0;
	Sphere* collision_ = nullptr;										//�R���W�������
	DebugSphere* debug_ = nullptr;										//�R���W�����\��
};