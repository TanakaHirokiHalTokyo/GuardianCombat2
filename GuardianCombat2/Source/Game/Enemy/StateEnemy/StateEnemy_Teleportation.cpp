#include "StateEnemy_Teleportation.h"
#include "../../Player/Player.h"
#include "../Enemy_Hige.h"
#include "../../GameManager/GameManager.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Effekseer/Effekseer.h"
#include "../../Blur/Blur.h"

void StateEnemy_Teleportation::Act(EnemyHige * enemy)
{
	//Parameter取得
	EnemyHigeTeleportation::ENEMY_PARAMETER parameter = enemy->GetTeleportParameter();

	if (!setVector_)
	{
		setVector_ = true;
		player_ = GameManager::GetPlayer();
		teleportVector_ = player_->GetVector()->GetFront() * -1.0f;			//ベクトルをセット
		GameManager::GetBlur()->SetUseBlur(true);									//ブラーを使う設定
		GameManager::GetBlur()->SetAlpha(150);										//ブラーのα値を設定
		GameManager::GetBlur()->SetStrength(1.1f);									//ブラーの強さを設定
	}
	else
	{
		//一定量潜ったらエフェクト再生してプレイヤーの後ろにテレポート
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
	//Parameter取得
	EnemyHigeTeleportation::ENEMY_PARAMETER parameter = enemy->GetTeleportParameter();

	parameter.effect->Draw();
}

void StateEnemy_Teleportation::EndDisplay(EnemyHige * enemy)
{
}
