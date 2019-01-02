#include "StateAction_Idle.h"
#include "../StateEnemy/StateEnemy.h"
#include "../StateEnemy/StateEnemy_MoveToPlayer.h"
#include "../StateEnemy/StateEnemy_SlowlyRotate.h"
#include "../Enemy_Hige.h"
#include "../Enemy.h"
#include "../../Player/Player.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>  
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
	//Parameter�擾
	EnemyIdle::ENEMY_PARAMETER parameter = enemy_->GetIdleParameter();
	if (enemy_->GetAutoAttack())
	{
		
		float rate = enemy_->GetLife() / ENEMY_MAX_LIFE;
		size_t i = 0;
		for (i = 0; i < parameter.hp_ratio_.size() + 1; i++)
		{
			if (i == parameter.hp_ratio_.size())break;
			if (rate >= parameter.hp_ratio_[i])
			{
				break;
			}
		}
		parameter.idle__counter++;
		if (parameter.idle__counter >= parameter.count[i])
		{
			double probability = parameter.normalAttackLuck[i];

			if ((double)rand() / RAND_MAX < probability) {
				//�ʏ�U���̏ꍇ
				int number = rand() % enemy_->GetNormalAttackNum() + 1;

				if (enemy_->GetEnemyType() == Enemy::ENEMY_HIGE)
				{
					EnemyHige* enemy = (EnemyHige*)enemy_;
					enemy->SetState((EnemyHige::STATE)number);
					return;
				}
			}
			else
			{
				if (enemy_->GetEnemyType() == Enemy::ENEMY_HIGE)
				{
					//����U���̏ꍇ
					int number = rand() % enemy_->GetSpecialAttackNum();

					EnemyHige* enemy = (EnemyHige*)enemy_;
					if (!(enemy->GetSummonsParameter().avater_alive > 0))
					{
						number = enemy->GetNormalAttackNum() + number + 1;
						enemy->SetState((EnemyHige::STATE)number);
					}
					else
					{
						//����U���̏ꍇ
						int number = rand() % (enemy_->GetSpecialAttackNum() - 1);
						number = enemy->GetNormalAttackNum() + number + 1;
						enemy->SetState((EnemyHige::STATE)number);
					}
					return;
				}
			}
			
		}
	}
	enemy_->SetIdleParameter(&parameter);

	if (this->IsApproach()) { move_->Act(enemy_); }				//�����𐧌�
	rotate_->Act(enemy_);			//��]����

	
}

void StateAction_Idle::BeginDisplay()
{
}

void StateAction_Idle::Display()
{
	move_->Display(enemy_);			//�f�o�b�O�\��
	rotate_->Display(enemy_);			//�f�o�b�O�\��
}

void StateAction_Idle::EndDisplay()
{
}

bool StateAction_Idle::IsApproach()
{
	//Parameter�擾
	EnemyIdle::ENEMY_PARAMETER parameter = enemy_->GetIdleParameter();

	Player* player = GameManager::GetPlayer();
	const auto& player_position = player->GetPosition();
	const auto& enemy_position = enemy_->GetPosition();

	auto length = player_position - enemy_position;
	

	return parameter.approache_length * parameter.approache_length < D3DXVec3LengthSq(&length);
}
