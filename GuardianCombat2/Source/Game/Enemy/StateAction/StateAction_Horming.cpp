#include "StateAction_Horming.h"
#include "../StateEnemy/StateEnemy_Horming.h"
#include "../StateEnemy/StateEnemy_Rotate.h"

StateAction_Horming::StateAction_Horming(Enemy * enemy)
{
	SetEnemy(enemy);		//“G‚ðÝ’è
	horming_ = new StateEnemy_Horming();
	rotate_ = new StateEnemy_Rotate();
}

StateAction_Horming::~StateAction_Horming()
{
	delete horming_;
	horming_ = nullptr;

	delete rotate_;
	rotate_ = nullptr;
}

void StateAction_Horming::Action()
{
	if (!rotateEnemy_)
	{
		rotateEnemy_ = true;
		rotate_->Act(enemy_);
	}

	horming_->Act((EnemyHige*)enemy_);
}

void StateAction_Horming::BeginDisplay()
{
	horming_->BeginDisplay((EnemyHige*)enemy_);
}

void StateAction_Horming::Display()
{
	horming_->Display((EnemyHige*)enemy_);
}

void StateAction_Horming::EndDisplay()
{
	horming_->EndDisplay((EnemyHige*)enemy_);
}
