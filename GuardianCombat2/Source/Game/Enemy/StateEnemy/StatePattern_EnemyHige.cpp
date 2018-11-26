#include "StatePattern_EnemyHige.h"
#include "../Enemy_Hige.h"
#include "../StateAction/StateAction.h"
#include "../StateAction/StateAction_Idle.h"
#include "../StateAction/StateAction_Rush.h"
#include "../StateAction/StateAction_CircleShot.h"
#include "../StateAction/StateAction_Horming.h"
#include "../StateAction/StateAction_Teleportation.h"
#include "../StateAction/StateAction_Summons.h"
#include "../StateAction/StateAction_Burst.h"
#include "../Enemy_HigeAvater.h"
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
	case EnemyHige::STATE::TELEPORTETION:
		action_ = new StateAction_Teleportation(enemy_);	//テレポート作成
		break;
	case EnemyHige::STATE::SUMMONSAVATER:
	{
		//パラメータ取得
		EnemyHigeSummons::ENEMY_PARAMETER parameter = enemy_->GetSummonsParameter();
		if (parameter.avater_alive > 0)
		{
			for (size_t i = 0; i < parameter.summons_max; i++)
			{
				parameter.avater[i].Init();
				parameter.avater[i].SetVisible(false);
			}
		}
		action_ = new StateAction_Summons(enemy_);		//分身召喚作成
		parameter.avater_alive = parameter.summons_num;
		enemy_->SetSummonsParameter(&parameter);
		break;
	}
	case EnemyHige::STATE::BURSTSHOT:
		action_ = new StateAction_Burst(enemy_);
		break;
	default:
		action_ = new StateAction_Idle(enemy_);			//待機状態作成
		break;
	}
}

void StatePattern_EnemyHige::SetEnemyHige(EnemyHige * enemy)
{
	//敵を設定
	enemy_ = enemy;
}
