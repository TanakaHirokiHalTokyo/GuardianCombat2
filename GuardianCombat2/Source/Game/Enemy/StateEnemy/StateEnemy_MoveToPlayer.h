#pragma once

//================================================================================
//
//		�G�̈ړ�����
//			�v���C���[�ɂ������߂Â��Ă���
//
//================================================================================

#include "StateEnemy.h"

class StateEnemy_MoveToPlayer :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
	void Display(Enemy* enemy)override;
};