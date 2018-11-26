#include "StateAction_Burst.h"
#include "../Enemy_Hige.h"
#include "../StateEnemy/StateEnemy_Rotate.h"
#include "../StateEnemy/StateEnemy_Burst.h"

StateAction_Burst::StateAction_Burst(Enemy* enemy)
{
	SetEnemy(enemy);
	rotate_ = new StateEnemy_Rotate();
	burst_ = new StateEnemy_Burst();
}

StateAction_Burst::~StateAction_Burst()
{
	if (rotate_)
	{
		delete rotate_;
		rotate_ = nullptr;
	}
	if (burst_)
	{
		delete burst_;
		burst_ = nullptr;
	}
}

void StateAction_Burst::Action()
{
	if (!alreadyrot_)
	{
		alreadyrot_ = true;
		rotate_->Act(enemy_);
	}
	else
	{
		burst_->Act((EnemyHige*)enemy_);
	}
}

void StateAction_Burst::BeginDisplay()
{
	if (!alreadyrot_)
	{
		rotate_->BeginDisplay(enemy_);
	}
}

void StateAction_Burst::Display()
{
	if (!alreadyrot_)
	{
		rotate_->Display(enemy_);
	}
	else
	{
		burst_->Display((EnemyHige*)enemy_);
	}
}

void StateAction_Burst::EndDisplay()
{
	if (!alreadyrot_)
	{
		rotate_->EndDisplay(enemy_);
	}
}
