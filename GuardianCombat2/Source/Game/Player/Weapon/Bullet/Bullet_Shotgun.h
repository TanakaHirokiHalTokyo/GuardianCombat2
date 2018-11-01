#pragma once

//================================================================
//
//		Shotgun�̒e�N���X
//
//================================================================

#include "Bullet.h"
class CEffekseer;

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
private:
	CEffekseer* bullet_[BulletNum];		//�U�e�̃G�t�F�N�g
	bool shot_ = false;					//���˒���
};