#pragma once

//======================================================
//
//		状態を実際に実行する
//
//======================================================

class Enemy;

class StateAction
{
public:
	virtual ~StateAction() {};					//デストラクタ
	virtual void Action() = 0;					//実行
	virtual void SetEnemy(Enemy* enemy) = 0;	//敵をセット
};