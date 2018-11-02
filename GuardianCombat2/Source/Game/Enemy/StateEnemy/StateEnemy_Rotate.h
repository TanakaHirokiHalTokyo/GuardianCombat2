#pragma once

//========================================================================
//
//			“G‚Ì‰ñ“]§Œä
//					‘¦U‚èŒü‚«
//========================================================================

#include "StateEnemy.h"

class StateEnemy_Rotate :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
	void Display(Enemy* enemy)override;
};