#pragma once

//==================================================
//
//		ë“ã@èÛë‘Çé¿çs
//
//==================================================

#include "StateAction.h"
class StateEnemy;

class StateAction_Idle :public StateAction
{
public:
	StateAction_Idle(Enemy* enemy);
	~StateAction_Idle()override;
	void Action()override;
	void SetEnemy(Enemy* enemy)override;
private:
	StateEnemy* move_ = nullptr;
	StateEnemy* rotate_ = nullptr;
	Enemy* enemy_ = nullptr;
};