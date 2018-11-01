#pragma once

//====================================================
//
//		WeaponƒNƒ‰ƒXEShotgun
//
//====================================================

#include "Weapon.h"
class Bullet;

class Weapon_Shotgun :public Weapon
{
public:
	Weapon_Shotgun();
	~Weapon_Shotgun();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

private:
	Bullet* bullet_ = nullptr;		//’e‚Ìî•ñ
};