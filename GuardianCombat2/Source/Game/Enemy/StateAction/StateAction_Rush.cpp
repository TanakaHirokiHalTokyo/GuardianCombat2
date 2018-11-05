#include "StateAction_Rush.h"
#include "../StateEnemy/StateEnemy_Rotate.h"
#include "../StateEnemy/StateEnemy_Rush.h"

StateAction_Rush::StateAction_Rush(Enemy* enemy)
{
	SetEnemy(enemy);		//�G�ݒ�

	//��]����쐬
	rotate_ = new StateEnemy_Rotate();
	//�ːi����쐬
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
	//�v���C���[�̂ق����܂��U������Ă��Ȃ��Ƃ��U������������s���B
	if (!isRotate_)
	{
		//��]�����@���s
		rotate_->Act(enemy_);
		isRotate_ = true;;
	}
	else
	{
		EnemyHige* hige = (EnemyHige*)enemy_;
		//�ːi�����@���s
		rush_->Act(hige);
	}
}

void StateAction_Rush::Display()
{
	EnemyHige* hige = (EnemyHige*)enemy_;
	//��]�����@�\��
	rotate_->Display(enemy_);
	rush_->Display(hige);
}

void StateAction_Rush::SetEnemy(Enemy * enemy)
{
	enemy_ = enemy;
}
