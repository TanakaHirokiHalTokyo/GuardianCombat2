#pragma once

//==============================================================
//
//		“G‚Ì‰ñ“]§Œä
//				‚ä‚Á‚­‚èU‚èŒü‚«
//
//==============================================================

#include "StateEnemy.h"

class StateEnemy_SlowlyRotate :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
};