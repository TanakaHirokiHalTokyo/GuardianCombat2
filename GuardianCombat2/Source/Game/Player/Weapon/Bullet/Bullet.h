#pragma once

//=====================================================================
//
//		BulletƒNƒ‰ƒX
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

	//UŒ‚—Í‚ğİ’èEæ“¾
	void SetAttack(float value) { attack_ = value; }
	float GetAttack() { return attack_; }
	
	//ˆê•bŠÔ‚É‘Å‚Ä‚é‰ñ”
	void SetRate(float value) { rate_ = value; }
	float GetRate() { return rate_; }

	//”­Ë‚·‚é•Ší‚ğİ’èEæ“¾
	void SetWeapon(Weapon* weapon) { weapon_ = weapon; }
	Weapon* GetWeapon() { return weapon_; }

protected:
	float attack_ = 1.0f;			//UŒ‚—Í
	float rate_ = 1.0f;				//ˆê•bŠÔ‚É‘Å‚Ä‚é‰ñ”

	Weapon* weapon_ = nullptr;			//”­Ë‚·‚é•Ší
};