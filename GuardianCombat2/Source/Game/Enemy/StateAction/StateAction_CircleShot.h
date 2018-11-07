#pragma once

//==========================================================
//
//		îgèÛçUåÇÅ@êßå‰
//
//==========================================================

#include "StateAction.h"

class StateEnemyHige;

class StateAction_CircleShot :public StateAction
{
public:
	StateAction_CircleShot(Enemy* enemy);
	~StateAction_CircleShot()override;
	void Action()override;
	void BeginDisplay()override;
	void Display()override;
	void EndDisplay()override;
private:
	StateEnemyHige* circleshot_ = nullptr;		//îgèÛçUåÇêßå‰
};