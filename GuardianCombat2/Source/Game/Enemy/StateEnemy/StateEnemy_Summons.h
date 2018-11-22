#pragma once

//======================================================================
//
//			分身召喚
//
//======================================================================

#include "StateEnemyHige.h"
class StateEnemy_Summons :public StateEnemyHige
{
public:
	StateEnemy_Summons();													//コンストラクタ
	~StateEnemy_Summons();													//デストラクタ
	void Act(EnemyHige* enemy)override;								//実行
	void BeginDisplay(EnemyHige* enemy)override;				//事前表示
	void Display(EnemyHige* enemy)override;							//表示
	void EndDisplay(EnemyHige* enemy)override;					//表示終了

private:
	EnemyHige* enemy_ = nullptr;						//敵のポインタ
	bool init_ = false;											//移動方向を設定したか
	float transValue_ = 0.0f;									//移動した合計量
	bool completed_ = false;
};