#pragma once

//========================================================================
//
//			�G�̉�]����
//					���U�����
//========================================================================

#include "StateEnemy.h"

class StateEnemy_Rotate :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
	void BeginDisplay(Enemy* enemy)override;
	void Display(Enemy* enemy)override;
	void EndDisplay(Enemy* enemy)override;
};