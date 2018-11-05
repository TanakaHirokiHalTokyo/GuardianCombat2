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
	void Display()override;					//デバッグ・パラメータ表示
	void SetEnemy(Enemy* enemy)override;	//敵を設定

private:
	StateEnemy* rotate_ = nullptr;			//プレイヤーのほうに振り向き
	StateEnemyHige* rush_ = nullptr;		//突進制御
	Enemy* enemy_ = nullptr;				//敵の情報
	bool isRotate_ = false;					//プレイヤーのほうを振り向いたか
};