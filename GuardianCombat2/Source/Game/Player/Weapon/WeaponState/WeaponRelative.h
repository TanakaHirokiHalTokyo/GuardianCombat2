#pragma once

//======================================================
//
//		����̑��ΐ���
//
//======================================================

#include "WeaponState.h"

class WeaponRelative :public WeaponState
{
public:
	void Act(Weapon* weapon)override;
};