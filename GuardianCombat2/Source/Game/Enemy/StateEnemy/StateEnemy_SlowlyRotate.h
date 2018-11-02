#pragma once

//==============================================================
//
//		�G�̉�]����
//				�������U�����
//
//==============================================================

#include "StateEnemy.h"

constexpr float ROTATE_ANGLE = 0.5f;					//�U��������x

class StateEnemy_SlowlyRotate :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
	void Display(Enemy* enemy)override;
private:
	float rotateAngle_ = ROTATE_ANGLE;
};