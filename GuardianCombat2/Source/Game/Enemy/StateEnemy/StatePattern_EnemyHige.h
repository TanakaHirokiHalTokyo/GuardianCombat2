#pragma once

//================================================
//
//		敵（ヒゲ）の状態変化による行動制御
//
//================================================

#include "StatePattern_Enemy.h"

class StateAction;
class EnemyHige;

class StatePattern_EnemyHige :public StatePattern_Enemy
{
public:
	StatePattern_EnemyHige(EnemyHige * enemy);		//コンストラクタ
	~StatePattern_EnemyHige()override;				//デストラクタ
	void Act()override;								//実行
	void ChangeState()override;						//状態変更

	void SetEnemyHige(EnemyHige* enemy);			//敵を設定
private:
	StatePattern_EnemyHige(){}						//引数ありコンストラクタだけを呼ぶように

	EnemyHige* enemy_ = nullptr;					//敵の情報
	StateAction* action_ = nullptr;					//実行情報
};