#include "StateEnemy_Teleportation.h"
#include "../../Player/Player.h"
#include "../Enemy_Hige.h"
#include "../../GameManager/GameManager.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Effekseer/Effekseer.h"
#include "../../Blur/Blur.h"

void StateEnemy_Teleportation::Act(EnemyHige * enemy)
{
	//Parameter�擾
	EnemyHigeTeleportation::ENEMY_PARAMETER parameter = enemy->GetTeleportParameter();

	if (!setVector_)
	{
		setVector_ = true;
		player_ = GameManager::GetPlayer();
		teleportVector_ = player_->GetVector()->GetFront() * -1.0f;			//�x�N�g�����Z�b�g
		GameManager::GetBlur()->SetUseBlur(true);									//�u���[���g���ݒ�
		GameManager::GetBlur()->SetAlpha(150);										//�u���[�̃��l��ݒ�
		GameManager::GetBlur()->SetStrength(1.1f);									//�u���[�̋�����ݒ�
	}
	else
	{
		//���ʐ�������G�t�F�N�g�Đ����ăv���C���[�̌��Ƀe���|�[�g
		if (divingValue_ >= diveValue_)															
		{
			if (!playEffect_)								
			{
				playEffect_ = true;
				parameter.effect->SetVisible(true);
				parameter.effect->SetPosition(enemy->GetPosition().x, 0.0f, enemy->GetPosition().z);
				enemy->SetPosition(player_->GetPosition() + teleportVector_ * parameter.distance);
				enemy->SetPositionY(0.0f);
				parameter.effect->Play();
			}
			else
			{
				if (!parameter.effect->GetPlaying())
				{
					enemy->FinishState();
					GameManager::GetBlur()->SetUseBlur(false);
				}
			}
		}
		else
		{
			divingValue_ += DIVE_SPEED;
			enemy->SetPositionY(enemy->GetPosition().y - DIVE_SPEED);
		}
	}
	parameter.effect->Update();
}

void StateEnemy_Teleportation::BeginDisplay(EnemyHige * enemy)
{
	
}

void StateEnemy_Teleportation::Display(EnemyHige * enemy)
{
	//Parameter�擾
	EnemyHigeTeleportation::ENEMY_PARAMETER parameter = enemy->GetTeleportParameter();

	parameter.effect->Draw();
}

void StateEnemy_Teleportation::EndDisplay(EnemyHige * enemy)
{
}
