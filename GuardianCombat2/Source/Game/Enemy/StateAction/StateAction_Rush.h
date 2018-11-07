#pragma once

//=======================================================================
//
//		突進行動　制御
//
//=======================================================================

#include "StateAction.h"

class StateEnemy;
class StateEnemyHige;

class StateAction_Rush :public StateAction
{
public:
	StateAction_Rush(Enemy* enemy);			//コンストラクタ
	~StateAction_Rush()override;			//デストラクタ
	void Action()override;					//実行
	void BeginDisplay()override;			//事前表示
	void Display()override;					//デバッグ・パラメータ表示
	void EndDisplay()override;				//表示終了

private:
	StateEnemy* rotate_ = nullptr;			//プレイヤーのほうに振り向き
	StateEnemyHige* rush_ = nullptr;		//突進制御
	bool isRotate_ = false;					//プレイヤーのほうを振り向いたか
};