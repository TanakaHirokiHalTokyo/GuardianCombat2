#pragma once

//=================================================
//
//		ホーミング実行
//
//=================================================
#include "StateEnemyHige.h"

class StateEnemy_Horming :public StateEnemyHige
{
public:
	void Act(EnemyHige* enemy)override;
	void BeginDisplay(EnemyHige* enemy)override;
	void Display(EnemyHige* enemy)override;
	void EndDisplay(EnemyHige* enemy)override;
private:
	bool spawn_ = false;				//キューブをスポーンさせたか
	bool setPosition_ = false;			//キューブを配置したか
	float transPositionValue_ = 0.0f;	//ポジションに達するまでに移動した距離
};
