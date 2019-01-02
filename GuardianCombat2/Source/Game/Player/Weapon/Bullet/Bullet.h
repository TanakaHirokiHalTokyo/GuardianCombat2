#pragma once

//=====================================================================
//
//		Bulletクラス
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
		Billboard* bullet_ = nullptr;													//弾のビルボード
		Sphere* collision_ = nullptr;												//コリジョン情報
		DebugSphere* debug_ = nullptr;										//コリジョン表示
		float speed_ = 0.1f;																//弾のスピード
		bool use_ = false;																//使用しているのか
		size_t count_ = 0;																//表示カウンタ
		D3DXVECTOR3 vector_ = D3DXVECTOR3(0, 0, 0);				//弾の飛んでいくベクトル								
	};

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	//攻撃力を設定・取得
	void SetAttack(float value) { attack_ = value; }
	float GetAttack() { return attack_; }
	
	//一秒間に打てる回数
	void SetRate(float value) { rate_ = value; }
	float GetRate() { return rate_; }

	//発射する武器を設定・取得
	void SetWeapon(Weapon* weapon) { weapon_ = weapon; }
	Weapon* GetWeapon() { return weapon_; }

	void SetShooting(D3DXVECTOR3 pos, D3DXVECTOR3 vec, float speed);

	bool GetShooting()
	{
		return bulletInfo_.use_;
	}

	void Destroy();

protected:
	BulletInfo bulletInfo_ = {};										//弾の情報
	float attack_ = 1.0f;													//攻撃力
	float rate_ = 1.0f;														//一秒間に打てる回数
	Weapon* weapon_ = nullptr;									//発射する武器
	bool use_ = false;													//使用しているのか
};