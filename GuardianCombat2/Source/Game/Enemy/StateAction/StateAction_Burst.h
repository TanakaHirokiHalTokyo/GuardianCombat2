#pragma once

//=====================================================
//
//			バーストショットの制御
//
//=======================================================

#include "StateAction.h"

class StateEnemy;
class StateEnemyHige;

class StateAction_Burst :public StateAction
{
public:
	StateAction_Burst(Enemy* enemy);
	~StateAction_Burst()override;
	void Action()override;
	void BeginDisplay()override;
	void Display()override;
	void EndDisplay()override;

private:
	bool alreadyrot_ = false;								//すでに振り向いているのか
	StateEnemy* rotate_ = nullptr;					//回転実行
	StateEnemyHige* burst_ = nullptr;				//バースト実行
};