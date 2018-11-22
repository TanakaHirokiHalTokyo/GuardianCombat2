#include "StateEnemy_MoveToPoint.h"
#include "../Enemy_Hige.h"
#include "../../../Vector3/Vector3.h"

StateEnemy_MoveToPoint::StateEnemy_MoveToPoint(D3DXVECTOR3 point,float speed)
{
	//移動先設定
	point_ = point;
	//スピード設定
	speed_ = speed;
}

StateEnemy_MoveToPoint::~StateEnemy_MoveToPoint()
{

}

void StateEnemy_MoveToPoint::Act(Enemy * enemy)
{
	//まだ移動先に到達していないとき実行
	if (!arrival_)
	{
		//移動先までのスピード設定
		speed_ = enemy->GetMoveSpeedToPoint();

		//移動先までの距離を求めていない際、距離を求める。
		if (!askLength_)
		{
			askLength_ = true;
			D3DXVECTOR3 enemy_position = enemy->GetPosition();
			D3DXVECTOR3 length_vector = point_ - enemy_position;
			length_ = D3DXVec3Length(&length_vector);
		}
		else
		{
			//移動先までの距離がスピード値よりも少ない場合、移動先までの距離をスピードとして扱う
			if (length_ - transValue_ < speed_) { 
				transValue_ += length_ - transValue_; 
				enemy->SetPosition(enemy->GetPosition() + enemy->GetVector()->GetFront() * (length_ - transValue_)); 
			}
			else { 
				transValue_ += speed_; 
				enemy->SetPosition(enemy->GetPosition() + enemy->GetVector()->GetFront() * speed_);
			}

			//移動先に到達した際にフラグ更新
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
