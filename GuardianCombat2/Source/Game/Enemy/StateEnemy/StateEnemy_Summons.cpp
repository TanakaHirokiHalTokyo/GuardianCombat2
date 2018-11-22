#include "StateEnemy_Summons.h"
#include "../Enemy_Hige.h"
#include "../Enemy_HigeAvater.h"
#include "../../../Vector3/Vector3.h"

StateEnemy_Summons::StateEnemy_Summons()
{
	
}

StateEnemy_Summons::~StateEnemy_Summons()
{
	
}

void StateEnemy_Summons::Act(EnemyHige * enemy)
{
	if (enemy_ == nullptr){	enemy_ = enemy;	}

	//パラメータ取得
	EnemyHigeSummons::ENEMY_PARAMETER parameter = enemy->GetSummonsParameter();
	
	//初期化をしていない場合
	if (!init_)
	{
		init_ = true;
		for (size_t i = 0; i < parameter.summons_num; i++)
		{
			parameter.avater[i].Init();
			parameter.avater[i].SetVisible(true);
			parameter.avater[i].SetFront(enemy->GetVector()->GetFront());
			parameter.avater[i].SetRight(enemy->GetVector()->GetRight());
			parameter.avater[i].SetUp(enemy->GetVector()->GetUp());
			parameter.avater[i].SetRotation(enemy_->GetRotate());

			//ベクトルを設定
			float angle = 360.0f / (float)(parameter.summons_num + 1);
			D3DXVECTOR3 enemy_front = enemy->GetVector()->GetFront();
			D3DXMATRIX mtxRotate;
			D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(angle * i));
			D3DXVec3TransformNormal(&enemy_front, &enemy_front, &mtxRotate);
			D3DXVec3Normalize(&enemy_front, &enemy_front);
			parameter.avater[i].SetSpawnVector(enemy_front);

			parameter.avater[i].Update();
		}
	}
	else
	{
		float speed;
		if (parameter.trans_length - transValue_ < enemy->GetMoveSpeedToPoint())
		{ 
			speed = parameter.trans_length - transValue_;
			completed_ = true;
		}
		else { speed = enemy->GetMoveSpeedToPoint();  }

		transValue_ += speed;
		for (size_t i = 0; i < parameter.summons_num; i++)
		{
			parameter.avater[i].SetMoving(true);
			//分身移動
			parameter.avater[i].SetPosition(parameter.avater[i].GetPosition() + parameter.avater[i].GetSpawnVector() * speed);

			if (completed_)
			{
				parameter.avater[i].SetMoving(false);
				enemy->FinishState();
			}
		}
	}

	//パラメータ設定
	enemy->SetSummonsParameter(&parameter);
}

void StateEnemy_Summons::BeginDisplay(EnemyHige * enemy)
{

}

void StateEnemy_Summons::Display(EnemyHige * enemy)
{

}

void StateEnemy_Summons::EndDisplay(EnemyHige * enemy)
{

}
