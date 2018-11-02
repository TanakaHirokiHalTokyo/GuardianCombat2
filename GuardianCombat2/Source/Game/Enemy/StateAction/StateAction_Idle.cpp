#include "StateAction_Idle.h"
#include "../StateEnemy/StateEnemy.h"
#include "../StateEnemy/StateEnemy_MoveToPlayer.h"
#include "../StateEnemy/StateEnemy_SlowlyRotate.h"

StateAction_Idle::StateAction_Idle(Enemy* enemy)
{
	SetEnemy(enemy);
	move_ = new StateEnemy_MoveToPlayer();		//プレイヤーを追尾する制御
	rotate_ = new StateEnemy_SlowlyRotate();	//ゆっくりプレイヤーのほうを向く制御
}

StateAction_Idle::~StateAction_Idle()
{
	if (move_)
	{
		delete move_;
		move_ = nullptr;
	}
	if (rotate_)
	{
		delete rotate_;
		rotate_ = nullptr;
	}
}

void StateAction_Idle::Action()
{
	move_->Act(enemy_);
	rotate_->Act(enemy_);
}

void StateAction_Idle::SetEnemy(Enemy * enemy)
{
	enemy_ = enemy;
}
