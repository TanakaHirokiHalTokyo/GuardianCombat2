#pragma once

//==============================================================
//
//		�G�̉�]����
//				�������U�����
//
//==============================================================

#include "StateEnemy.h"

class StateEnemy_SlowlyRotate :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
};