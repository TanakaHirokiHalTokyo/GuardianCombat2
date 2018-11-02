#pragma once

//================================================================================
//
//		“G‚ÌˆÚ“®§Œä
//			ƒvƒŒƒCƒ„[‚É‚ä‚Á‚­‚è‹ß‚Ã‚¢‚Ä‚­‚é
//
//================================================================================

#include "StateEnemy.h"

class StateEnemy_MoveToPlayer :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
	void Display(Enemy* enemy)override;
};