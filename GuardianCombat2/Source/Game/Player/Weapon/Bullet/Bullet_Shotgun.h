#pragma once

//================================================================
//
//		Shotgunの弾クラス
//
//================================================================

#include "Bullet.h"
class CEffekseer;
class Sphere;
class DebugSphere;

const unsigned int BulletNum = 6;		//散弾する弾の数

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
	CEffekseer* bullet_[BulletNum];										//散弾のエフェクト
	D3DXVECTOR3 bulletVector_[BulletNum] = { D3DXVECTOR3(0,0,0) };		//弾の発射向き
	bool shot_ = false;													//発射中か
	int count_ = 0;
	Sphere* collision_ = nullptr;										//コリジョン情報
	DebugSphere* debug_ = nullptr;										//コリジョン表示
};