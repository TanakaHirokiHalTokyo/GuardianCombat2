#pragma once

//==============================================================
//
//		敵の回転制御
//				ゆっくり振り向き
//
//==============================================================

#include "StateEnemy.h"

constexpr float ROTATE_ANGLE = 0.5f;					//振り向き速度

class StateEnemy_SlowlyRotate :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;
	void BeginDisplay(Enemy* enemy)override;
	void Display(Enemy* enemy)override;
	void EndDisplay(Enemy* enemy)override;
private:
	float rotateAngle_ = ROTATE_ANGLE;					//振り向き角度
};