#pragma once

//=====================================================
//
//		Horming制御
//
//=====================================================

#include "StateAction.h"
class StateEnemyHige;
class StateEnemy;

class StateAction_Horming :public StateAction
{
public:
	StateAction_Horming(Enemy* enemy);		//コンストラクタ
	~StateAction_Horming()override;			//デストラクタ
	void Action()override;					//実行関数
	void BeginDisplay()override;			//事前表示
	void Display()override;					//表示
	void EndDisplay()override;				//表示終了

private:
	StateEnemyHige* horming_ = nullptr;			//ホーミング実行
	StateEnemy* rotate_ = nullptr;				//プレイヤーのほうを向く制御
	bool rotateEnemy_ = false;
};