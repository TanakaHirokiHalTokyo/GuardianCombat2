#pragma once

//=====================================================================
//
//		Bulletクラス
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

	//攻撃力を設定・取得
	void SetAttack(float value) { attack_ = value; }
	float GetAttack() { return attack_; }
	
	//一秒間に打てる回数
	void SetRate(float value) { rate_ = value; }
	float GetRate() { return rate_; }

	//発射する武器を設定・取得
	void SetWeapon(Weapon* weapon) { weapon_ = weapon; }
	Weapon* GetWeapon() { return weapon_; }

protected:
	float attack_ = 1.0f;			//攻撃力
	float rate_ = 1.0f;				//一秒間に打てる回数
	Weapon* weapon_ = nullptr;		//発射する武器
	
};