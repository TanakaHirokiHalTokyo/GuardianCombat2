#pragma once
//=================================================================
//
//			真ん中を向かせる処理
//
//=================================================================

#include "StateEnemy.h"

class StateEnemy_RotateToMiddle :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;							//状態実行
	void BeginDisplay(Enemy* enemy)override;			//事前表示
	void Display(Enemy* enemy)override;					//表示
	void EndDisplay(Enemy* enemy)override;				//終了表示
private:

};