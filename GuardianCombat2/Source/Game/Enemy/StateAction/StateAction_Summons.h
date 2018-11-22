#pragma once
//=============================================================================
//
//			分身召喚制御
//
//=============================================================================

#include "StateAction.h"
class StateEnemy;
class StateEnemyHige;
class StateAction_Summons :public StateAction
{
public:
	StateAction_Summons(Enemy* enemy);				//コンストラクタ
	~StateAction_Summons();										//デストラクタ
	void Action()override;												//実行
	void BeginDisplay()override;									//事前表示			
	void Display()override;											//表示
	void EndDisplay()override;										//終了表示
private:
	bool rotate_ = false;												//真ん中を向いたか
	StateEnemy* rotateMiddle_ = nullptr;					//真ん中に向かせる制御
	StateEnemy* transMiddle_ = nullptr;						//真ん中に移動させる制御
	StateEnemyHige* summons_ = nullptr;					//召喚制御
};