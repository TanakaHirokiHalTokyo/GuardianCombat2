#pragma once
//=================================================================
//
//			�o�[�X�g�V���b�g���s
//
//=================================================================

#include "StateEnemyHige.h"
class StateEnemyHige;
class StateEnemy_Burst :public StateEnemyHige
{
public:
	void Act(EnemyHige* enemy)override;
	void BeginDisplay(EnemyHige* enemy)override;
	void Display(EnemyHige* enemy)override;
	void EndDisplay(EnemyHige* enemy)override;
private:
	int count_ = 0;
	bool init_ = false;
};