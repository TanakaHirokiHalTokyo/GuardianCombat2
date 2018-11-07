#include "StateAction_CircleShot.h"
#include "../StateEnemy/StateEnemy_CircleShot.h"

StateAction_CircleShot::StateAction_CircleShot(Enemy* enemy)
{
	SetEnemy(enemy);									//“G‚ðÝ’è
	circleshot_ = new StateEnemy_CircleShot();			//§Œä‚ðì¬
}

StateAction_CircleShot::~StateAction_CircleShot()
{
	if (circleshot_)
	{
		delete circleshot_;
		circleshot_ = nullptr;
	}
}

void StateAction_CircleShot::Action()
{
	circleshot_->Act((EnemyHige*)enemy_);				//ŽÀs
}

void StateAction_CircleShot::BeginDisplay()
{
	circleshot_->BeginDisplay((EnemyHige*)enemy_);
}

void StateAction_CircleShot::Display()
{
	circleshot_->Display((EnemyHige*)enemy_);			//•\Ž¦
}

void StateAction_CircleShot::EndDisplay()
{
	circleshot_->EndDisplay((EnemyHige*)enemy_);
}
