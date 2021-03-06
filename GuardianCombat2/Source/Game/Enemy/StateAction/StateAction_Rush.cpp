#include "StateAction_Rush.h"
#include "../StateEnemy/StateEnemy_Rotate.h"
#include "../StateEnemy/StateEnemy_Rush.h"

StateAction_Rush::StateAction_Rush(Enemy* enemy)
{
	SetEnemy(enemy);		//敵設定

	//回転制御作成
	rotate_ = new StateEnemy_Rotate();
	//突進制御作成
	rush_ = new StateEnemy_Rush();
}

StateAction_Rush::~StateAction_Rush()
{
	if (rotate_)
	{
		delete rotate_;
		rotate_ = nullptr;
	}
	if (rush_)
	{
		delete rush_;
		rush_ = nullptr;
	}
}

void StateAction_Rush::Action()
{
	//プレイヤーのほうをまだ振り向いていないとき振り向き処理を行う。
	if (!isRotate_)
	{
		//回転処理　実行
		rotate_->Act(enemy_);
		isRotate_ = true;;
	}
	else
	{
		EnemyHige* hige = (EnemyHige*)enemy_;
		//突進処理　実行
		rush_->Act(hige);
	}
}

void StateAction_Rush::BeginDisplay()
{
}

void StateAction_Rush::Display()
{
	EnemyHige* hige = (EnemyHige*)enemy_;
	//回転処理　表示
	rotate_->Display(enemy_);
	rush_->Display(hige);
}

void StateAction_Rush::EndDisplay()
{
}

