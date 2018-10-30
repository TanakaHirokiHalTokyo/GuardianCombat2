#pragma once

//===============================================================
//
//		Weapon‚Ìó‘Ô‚ğ§Œä
//
//===============================================================

class Weapon;

class WeaponState
{
public:
	virtual void Act(Weapon* weapon) = 0;
};