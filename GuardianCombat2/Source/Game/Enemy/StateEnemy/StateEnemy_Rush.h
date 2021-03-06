#pragma once

//========================================================-
//
//		突進　制御
//
//=========================================================

#include "StateEnemyHige.h"

class StateEnemy_Rush :public StateEnemyHige
{
public:
	void Act(EnemyHige* enemy)override;
	void BeginDisplay(EnemyHige* enemy)override;
	void Display(EnemyHige* enemy)override;
	void EndDisplay(EnemyHige* enemy)override;
private:
	float rushLength_ = 0.0f;		//突進した距離
};