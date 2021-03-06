#pragma once

//===============================================================
//
//		Weaponの状態を制御
//
//===============================================================

class Weapon;

class WeaponState
{
public:
	virtual void Act(Weapon* weapon) = 0;
};