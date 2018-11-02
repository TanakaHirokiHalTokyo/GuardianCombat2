#pragma once

//======================================================================
//
//		敵の状態パターンによる行動制御
//
//======================================================================

class Enemy;

class StatePattern_Enemy
{
public:
	virtual ~StatePattern_Enemy() {};		//デストラクタ
	virtual void Act() = 0;					//実行
	virtual void Display() = 0;				//表示
	virtual void ChangeState() = 0;			//ステート変更
};