#pragma once

//======================================================
//
//		•Ší‚Ì‘Š‘Î§Œä
//
//======================================================

#include "WeaponState.h"

class WeaponRelative :public WeaponState
{
public:
	void Act(Weapon* weapon)override;
};