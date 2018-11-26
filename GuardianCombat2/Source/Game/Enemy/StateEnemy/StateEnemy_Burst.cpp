#include "StateEnemy_Burst.h"
#include "../Enemy_Hige.h"
#include "../../../Effekseer/Effekseer.h"
#include "../../Blur/Blur.h"
#include "../../GameManager/GameManager.h"	
#include "../../../Vector3/Vector3.h"

void StateEnemy_Burst::Act(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeBurstShot::ENEMY_PARAMETER parameter = enemy->GetBurstParameter();

	if (!init_)
	{
		init_ = true;
		parameter.effect->SetVisible(true);
		parameter.effect->SetPosition(enemy->GetPosition().x, enemy->GetPosition().y + parameter.effect_size, enemy->GetPosition().z);
		parameter.effect->SetPosition(parameter.effect->GetPosition() + enemy->GetVector()->GetFront() * 0.5f);
		parameter.effect->SetRotate(enemy->GetRotate().x, enemy->GetRotate().y + 180.0f, enemy->GetRotate().z);
		parameter.effect->Play();

		parameter.collision->m_Pos = parameter.effect->GetPosition() + enemy->GetVector()->GetFront() * (EnemyHige::BURSTZ * parameter.effect->GetScale().z) * 0.5f;
		parameter.collision->m_fLength[0] = parameter.effect_size * EnemyHige::BURSTX;
		parameter.collision->m_fLength[1] = parameter.effect_size * EnemyHige::BURSTY;
		parameter.collision->m_fLength[2] = parameter.effect_size * EnemyHige::BURSTZ;
		parameter.collision->m_NormaDirect[0] = enemy->GetVector()->GetRight();
		parameter.collision->m_NormaDirect[1] = enemy->GetVector()->GetUp();
		parameter.collision->m_NormaDirect[2] = enemy->GetVector()->GetFront();

		Blur* blur = GameManager::GetBlur();
		blur->SetUseBlur(true);
		blur->SetAlpha(240);
		blur->SetColor(255,200,200);
		blur->SetStrength(1.0f);

	}
	if (!parameter.effect->GetPlaying())
	{
		Blur* blur = GameManager::GetBlur();
		blur->SetUseBlur(false);
		parameter.collision->enable_ = false;
		parameter.effect->SetVisible(false);
		enemy->SetBurstParameter(&parameter);
		enemy->FinishState();
		return;
	}
	count_++;
	if (count_ >= 60)
	{
		parameter.collision->enable_ = true;
	}
	parameter.effect->Update();
	parameter.effect->SetScale(parameter.effect_size, parameter.effect_size, parameter.effect_size);

	enemy->SetBurstParameter(&parameter);
}

void StateEnemy_Burst::BeginDisplay(EnemyHige * enemy)
{
	
}

void StateEnemy_Burst::Display(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeBurstShot::ENEMY_PARAMETER parameter = enemy->GetBurstParameter();

	if (parameter.effect->GetPlaying())
	{
		parameter.effect->Draw();
	}
}

void StateEnemy_Burst::EndDisplay(EnemyHige * enemy)
{
	
}
