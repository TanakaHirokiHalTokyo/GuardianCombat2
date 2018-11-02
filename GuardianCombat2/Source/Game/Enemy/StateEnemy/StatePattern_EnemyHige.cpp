#include "StatePattern_EnemyHige.h"
#include "../Enemy_Hige.h"
#include "../StateAction/StateAction.h"
#include "../StateAction/StateAction_Idle.h"
#include "../Enemy_Hige.h"

StatePattern_EnemyHige::StatePattern_EnemyHige(EnemyHige * enemy)
{
	SetEnemyHige(enemy);		//“G‚ğ“o˜^
	ChangeState();				//ƒXƒe[ƒg•ÏX
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
	action_->Action();		//Às
}

void StatePattern_EnemyHige::ChangeState()
{
	//ó‘Ô•ÏX
	switch (enemy_->GetState())
	{
	case EnemyHige::STATE::IDLE:
		action_ = new StateAction_Idle(enemy_);
		break;
	default:
		break;
	}
}

void StatePattern_EnemyHige::SetEnemyHige(EnemyHige * enemy)
{
	//“G‚ğİ’è
	enemy_ = enemy;
}
