#include "StateAction_CircleShot.h"
#include "../StateEnemy/StateEnemy_CircleShot.h"

StateAction_CircleShot::StateAction_CircleShot(Enemy* enemy)
{
	SetEnemy(enemy);									//敵を設定
	circleshot_ = new StateEnemy_CircleShot();			//制御を作成
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
	circleshot_->Act((EnemyHige*)enemy_);				//実行
}

void StateAction_CircleShot::BeginDisplay()
{
	circleshot_->BeginDisplay((EnemyHige*)enemy_);
}

void StateAction_CircleShot::Display()
{
	circleshot_->Display((EnemyHige*)enemy_);			//表示
}

void StateAction_CircleShot::EndDisplay()
{
	circleshot_->EndDisplay((EnemyHige*)enemy_);
}
