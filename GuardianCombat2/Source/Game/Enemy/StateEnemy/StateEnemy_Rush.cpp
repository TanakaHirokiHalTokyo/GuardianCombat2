#include "StateEnemy_Rush.h"
#include "../Enemy_Hige.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/Player.h"
#include "../Enemy.h"
#include "../../../Imgui/ImguiManager.h"
#include "../../../Vector3/Vector3.h"

void StateEnemy_Rush::Act(EnemyHige * enemy)
{
	//Player情報取得
	const auto& player_info = GameManager::GetPlayer();

	//パラメータ取得
	EnemyHigeRush::ENEMY_PARAMETER parameter = enemy->GetRushParameter();
	
	//座標更新
	enemy->SetPosition(enemy->GetPosition() + enemy->GetVector()->GetFront() * parameter.speed);

	//進んだ距離更新
	rushLength_ += parameter.speed;

	//設定した距離分突進したら突進終了
	if (rushLength_ > parameter.length)
	{
		//行動終了　IDLE状態に
		enemy->FinishState();
	}
}

void StateEnemy_Rush::Display(EnemyHige * enemy)
{
	//デバッグモード時
	if (enemy->GetDebugMode())
	{
		
	}
}
