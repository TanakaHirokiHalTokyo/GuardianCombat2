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
	move_->Act(enemy_);				//動きを制御
	rotate_->Act(enemy_);			//回転制御
}

void StateAction_Idle::BeginDisplay()
{
}

void StateAction_Idle::Display()
{
	move_->Display(enemy_);			//デバッグ表示
	rotate_->Display(enemy_);		//デバッグ表示
}

void StateAction_Idle::EndDisplay()
{
}
