#pragma once

//===============================================================
//
//		Weapon�̏�Ԃ𐧌�
//
//===============================================================

class Weapon;

class WeaponState
{
public:
	virtual void Act(Weapon* weapon) = 0;
};