#include "StateAction_Summons.h"
#include "../StateEnemy/StateEnemy_RotateToMiddle.h"
#include "../StateEnemy/StateEnemy_MoveToPoint.h"
#include "../StateEnemy/StateEnemy_Summons.h"
StateAction_Summons::StateAction_Summons(Enemy* enemy)
{
	SetEnemy(enemy);
	rotateMiddle_ = new StateEnemy_RotateToMiddle();
	transMiddle_ = new StateEnemy_MoveToPoint(D3DXVECTOR3(0,0,0),0.1f);
	summons_ = new StateEnemy_Summons();
}

StateAction_Summons::~StateAction_Summons()
{
	if (rotateMiddle_)
	{
		delete rotateMiddle_;
		rotateMiddle_ = nullptr;
	}
	if (transMiddle_)
	{
		delete transMiddle_;
		transMiddle_ = nullptr;
	}
	if (summons_)
	{
		delete summons_;
		summons_ = nullptr;
	}
}

void StateAction_Summons::Action()
{
	if (!rotate_)
	{
		rotate_ = true;
		rotateMiddle_->Act(enemy_);
	}
	else 
	{
		StateEnemy_MoveToPoint* move = (StateEnemy_MoveToPoint*)transMiddle_;
		if (!move->GetArrial())
		{
			transMiddle_->Act(enemy_);
		}
		else
		{
			summons_->Act((EnemyHige*)enemy_);
		}
	}
}

void StateAction_Summons::BeginDisplay()
{

}

void StateAction_Summons::Display()
{

}

void StateAction_Summons::EndDisplay()
{

}
