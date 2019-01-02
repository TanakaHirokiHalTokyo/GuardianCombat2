#pragma once

//==================================================
//
//		待機状態を実行
//
//==================================================

#include "StateAction.h"
class StateEnemy;

class StateAction_Idle :public StateAction
{
public:
	StateAction_Idle(Enemy* enemy);			//コンストラクタ
	~StateAction_Idle()override;			//デストラクタ
	void Action()override;					//実行
	void BeginDisplay()override;			//事前表示
	void Display()override;					//表示
	void EndDisplay()override;				//表示終了
private:
	bool IsApproach();							//これ以上プレイヤーに近づくかを判断

	StateEnemy* move_ = nullptr;			//移動を制御
	StateEnemy* rotate_ = nullptr;			//回転情報制御
};