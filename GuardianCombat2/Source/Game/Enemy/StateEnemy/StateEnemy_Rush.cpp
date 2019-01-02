#include "StateEnemy_Rush.h"
#include "../Enemy_Hige.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/Player.h"
#include "../Enemy.h"
#include "../../../Imgui/ImguiManager.h"
#include "../../../Vector3/Vector3.h"

void StateEnemy_Rush::Act(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeRush::ENEMY_PARAMETER parameter = enemy->GetRushParameter();

	const auto& vector = enemy->GetVector();
	enemy->SetCollisionEnable(true);
	enemy->SetCollisionDPS(parameter.attack);

	//座標更新
	enemy->SetPosition(enemy->GetPosition() + vector->GetFront() * parameter.speed);

	//進んだ距離更新
	rushLength_ += parameter.speed;

	//設定した距離分突進したら突進終了
	if (rushLength_ > parameter.length)
	{
		enemy->SetCollisionEnable(false);
		enemy->SetCollisionDPS(0.0f);
		//行動終了　IDLE状態に
		enemy->FinishState();
	}
}

void StateEnemy_Rush::BeginDisplay(EnemyHige * enemy)
{
}

void StateEnemy_Rush::Display(EnemyHige * enemy)
{

}

void StateEnemy_Rush::EndDisplay(EnemyHige * enemy)
{
}
