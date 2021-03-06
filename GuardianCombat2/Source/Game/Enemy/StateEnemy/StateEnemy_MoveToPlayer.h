#pragma once

//================================================================================
//
//		敵の移動制御
//			プレイヤーにゆっくり近づいてくる
//
//================================================================================

#include "StateEnemy.h"

class StateEnemy_MoveToPlayer :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
	void BeginDisplay(Enemy* enemy)override;
	void Display(Enemy* enemy)override;
	void EndDisplay(Enemy* enemy)override;	
};