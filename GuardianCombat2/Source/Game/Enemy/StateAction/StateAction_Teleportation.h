#pragma once
//=======================================================================
//
//		テレポート　制御
//
//=======================================================================
#include "StateAction.h"
class StateEnemyHige;

class StateAction_Teleportation :public StateAction
{
public:
	StateAction_Teleportation(Enemy* enemy);
	~StateAction_Teleportation()override;
	void Action()override;									//実行関数
	void BeginDisplay()override;						//事前表示
	void Display()override;								//表示
	void EndDisplay()override;							//表示終了
private:
	StateEnemyHige*  teleportation_ = nullptr;
};