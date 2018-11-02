#pragma once

//================================================================
//
//		ShotgunÌeNX
//
//================================================================

#include "Bullet.h"
class CEffekseer;

const unsigned int BulletNum = 6;		//Ue·éeÌ

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
	CEffekseer* bullet_[BulletNum];										//UeÌGtFNg
	D3DXVECTOR3 bulletVector_[BulletNum] = { D3DXVECTOR3(0,0,0) };		//eÌ­Ëü«
	bool shot_ = false;													//­Ë©
	int count_ = 0;
};