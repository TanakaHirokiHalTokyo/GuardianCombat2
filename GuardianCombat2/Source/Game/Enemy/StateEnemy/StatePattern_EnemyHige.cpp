#include "StatePattern_EnemyHige.h"
#include "../Enemy_Hige.h"
#include "../StateAction/StateAction.h"
#include "../StateAction/StateAction_Idle.h"
#include "../StateAction/StateAction_Rush.h"
#include "../StateAction/StateAction_CircleShot.h"
#include "../StateAction/StateAction_Horming.h"
#include "../Enemy_Hige.h"

StatePattern_EnemyHige::StatePattern_EnemyHige(EnemyHige * enemy)
{
	SetEnemyHige(enemy);		//敵を登録
	ChangeState();				//ステート変更
}

StatePattern_EnemyHige::~StatePattern_EnemyHige()
{
	if (action_)
	{
		delete action_;
	}
	
}

void StatePattern_EnemyHige::Act()
{
	action_->Action();		//実行
}

void StatePattern_EnemyHige::BeginDisplay()
{
	action_->BeginDisplay();
}

void StatePattern_EnemyHige::Display()
{
	action_->Display();		//パラメータ表示
}

void StatePattern_EnemyHige::EndDisplay()
{
	action_->EndDisplay();
}

void StatePattern_EnemyHige::ChangeState()
{
	//制御変更
	if (action_) { delete action_; action_ = nullptr; }

	//状態変更
	switch (enemy_->GetState())
	{
	case EnemyHige::STATE::IDLE:
		action_ = new StateAction_Idle(enemy_);			//待機状態作成
		break;
	case EnemyHige::STATE::RUSH:
		action_ = new StateAction_Rush(enemy_);			//突進状態作成
		break;
	case EnemyHige::STATE::CIRCLESHOT:
		action_ = new StateAction_CircleShot(enemy_);	//波状攻撃作成
		break;
	case EnemyHige::STATE::HORMING:
		action_ = new StateAction_Horming(enemy_);		//ホーミング作製
		break;
	default:
		break;
	}
}

void StatePattern_EnemyHige::SetEnemyHige(EnemyHige * enemy)
{
	//敵を設定
	enemy_ = enemy;
}
