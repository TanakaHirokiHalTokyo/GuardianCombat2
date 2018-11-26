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
	SetEnemyHige(enemy);		//�G��o�^
	ChangeState();				//�X�e�[�g�ύX
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
	action_->Action();		//���s
}

void StatePattern_EnemyHige::BeginDisplay()
{
	action_->BeginDisplay();
}

void StatePattern_EnemyHige::Display()
{
	action_->Display();		//�p�����[�^�\��
}

void StatePattern_EnemyHige::EndDisplay()
{
	action_->EndDisplay();
}

void StatePattern_EnemyHige::ChangeState()
{
	//����ύX
	if (action_) { delete action_; action_ = nullptr; }

	//��ԕύX
	switch (enemy_->GetState())
	{
	case EnemyHige::STATE::IDLE:
		action_ = new StateAction_Idle(enemy_);			//�ҋ@��ԍ쐬
		break;
	case EnemyHige::STATE::RUSH:
		action_ = new StateAction_Rush(enemy_);			//�ːi��ԍ쐬
		break;
	case EnemyHige::STATE::CIRCLESHOT:
		action_ = new StateAction_CircleShot(enemy_);	//�g��U���쐬
		break;
	case EnemyHige::STATE::HORMING:
		action_ = new StateAction_Horming(enemy_);		//�z�[�~���O�쐻
		break;
	case EnemyHige::STATE::TELEPORTETION:
		action_ = new StateAction_Teleportation(enemy_);	//�e���|�[�g�쐬
		break;
	case EnemyHige::STATE::SUMMONSAVATER:
	{
		//�p�����[�^�擾
		EnemyHigeSummons::ENEMY_PARAMETER parameter = enemy_->GetSummonsParameter();
		if (parameter.avater_alive > 0)
		{
			for (size_t i = 0; i < parameter.summons_max; i++)
			{
				parameter.avater[i].Init();
				parameter.avater[i].SetVisible(false);
			}
		}
		action_ = new StateAction_Summons(enemy_);		//���g�����쐬
		parameter.avater_alive = parameter.summons_num;
		enemy_->SetSummonsParameter(&parameter);
		break;
	}
	case EnemyHige::STATE::BURSTSHOT:
		action_ = new StateAction_Burst(enemy_);
		break;
	default:
		action_ = new StateAction_Idle(enemy_);			//�ҋ@��ԍ쐬
		break;
	}
}

void StatePattern_EnemyHige::SetEnemyHige(EnemyHige * enemy)
{
	//�G��ݒ�
	enemy_ = enemy;
}
