#include "StateEnemy_MoveToPoint.h"
#include "../Enemy_Hige.h"
#include "../../../Vector3/Vector3.h"

StateEnemy_MoveToPoint::StateEnemy_MoveToPoint(D3DXVECTOR3 point,float speed)
{
	//�ړ���ݒ�
	point_ = point;
	//�X�s�[�h�ݒ�
	speed_ = speed;
}

StateEnemy_MoveToPoint::~StateEnemy_MoveToPoint()
{

}

void StateEnemy_MoveToPoint::Act(Enemy * enemy)
{
	//�܂��ړ���ɓ��B���Ă��Ȃ��Ƃ����s
	if (!arrival_)
	{
		//�ړ���܂ł̃X�s�[�h�ݒ�
		speed_ = enemy->GetMoveSpeedToPoint();

		//�ړ���܂ł̋��������߂Ă��Ȃ��ہA���������߂�B
		if (!askLength_)
		{
			askLength_ = true;
			D3DXVECTOR3 enemy_position = enemy->GetPosition();
			D3DXVECTOR3 length_vector = point_ - enemy_position;
			length_ = D3DXVec3Length(&length_vector);
		}
		else
		{
			//�ړ���܂ł̋������X�s�[�h�l�������Ȃ��ꍇ�A�ړ���܂ł̋������X�s�[�h�Ƃ��Ĉ���
			if (length_ - transValue_ < speed_) { 
				transValue_ += length_ - transValue_; 
				enemy->SetPosition(enemy->GetPosition() + enemy->GetVector()->GetFront() * (length_ - transValue_)); 
			}
			else { 
				transValue_ += speed_; 
				enemy->SetPosition(enemy->GetPosition() + enemy->GetVector()->GetFront() * speed_);
			}

			//�ړ���ɓ��B�����ۂɃt���O�X�V
			if (transValue_ >= length_)
			{
				arrival_ = true;
			}
		}
	}
}

void StateEnemy_MoveToPoint::BeginDisplay(Enemy * enemy)
{
}

void StateEnemy_MoveToPoint::Display(Enemy * enemy)
{
}

void StateEnemy_MoveToPoint::EndDisplay(Enemy * enemy)
{
}

bool StateEnemy_MoveToPoint::GetArrial()
{
	return arrival_;
}
