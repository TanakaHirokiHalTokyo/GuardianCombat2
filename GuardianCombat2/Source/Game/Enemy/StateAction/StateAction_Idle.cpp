#include "StateAction_Idle.h"
#include "../StateEnemy/StateEnemy.h"
#include "../StateEnemy/StateEnemy_MoveToPlayer.h"
#include "../StateEnemy/StateEnemy_SlowlyRotate.h"

StateAction_Idle::StateAction_Idle(Enemy* enemy)
{
	SetEnemy(enemy);
	move_ = new StateEnemy_MoveToPlayer();		//�v���C���[��ǔ����鐧��
	rotate_ = new StateEnemy_SlowlyRotate();	//�������v���C���[�̂ق�����������
}

StateAction_Idle::~StateAction_Idle()
{
	if (move_)
	{
		delete move_;
		move_ = nullptr;
	}
	if (rotate_)
	{
		delete rotate_;
		rotate_ = nullptr;
	}
}

void StateAction_Idle::Action()
{
	move_->Act(enemy_);				//�����𐧌�
	rotate_->Act(enemy_);			//��]����
}

void StateAction_Idle::BeginDisplay()
{
}

void StateAction_Idle::Display()
{
	move_->Display(enemy_);			//�f�o�b�O�\��
	rotate_->Display(enemy_);		//�f�o�b�O�\��
}

void StateAction_Idle::EndDisplay()
{
}
