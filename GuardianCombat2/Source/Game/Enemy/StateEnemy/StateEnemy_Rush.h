#pragma once

//========================================================-
//
//		“Ëi@§Œä
//
//=========================================================

#include "StateEnemyHige.h"

class StateEnemy_Rush :public StateEnemyHige
{
public:
	void Act(EnemyHige* enemy)override;
	void Display(EnemyHige* enemy)override;
private:
	float rushLength_ = 0.0f;		//“Ëi‚µ‚½‹——£
};