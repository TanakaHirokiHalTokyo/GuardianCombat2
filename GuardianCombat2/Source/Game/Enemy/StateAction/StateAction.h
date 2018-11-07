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
	virtual ~StateAction() {};								//デストラクタ
	virtual void Action() = 0;								//実行
	virtual void BeginDisplay() = 0;						//事前表示
	virtual void Display() = 0;								//表示
	virtual void EndDisplay() = 0;							//表示終了
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }			//敵をセット
protected:
	Enemy* enemy_ = nullptr;
};