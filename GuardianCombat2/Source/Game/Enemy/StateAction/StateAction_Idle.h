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
	void Display()override;					//表示
	void SetEnemy(Enemy* enemy)override;	//敵を設定
private:
	StateEnemy* move_ = nullptr;			//移動を制御
	StateEnemy* rotate_ = nullptr;			//回転情報制御
	Enemy* enemy_ = nullptr;				//敵の情報
};