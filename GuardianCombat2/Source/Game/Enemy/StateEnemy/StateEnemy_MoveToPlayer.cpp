#include "StateEnemy_MoveToPlayer.h"
#include "../Enemy.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Imgui/ImguiManager.h"

void StateEnemy_MoveToPlayer::Act(Enemy * enemy)
{
	//敵のベクトル取得
	const auto& enemy_vector = enemy->GetVector();

	//座標を設定
	enemy->SetPosition(enemy->GetPosition() + enemy_vector->GetFront() * enemy->GetSpeed());
}

void StateEnemy_MoveToPlayer::Display(Enemy* enemy)
{
	//デバッグモード時
	if (enemy->GetDebugMode())
	{
		EnemyIdle::ENEMY_PARAMETER parameter = enemy->GetIdleParameter();

		//敵のスピード取得
		float speed = parameter.speed;

		//パラメータ表示
		ImGui::Begin("EnemyDebugParameter");
		ImGui::DragFloat("Speed Set", &speed, 0.001f, 0.0f, 0.1f);
		ImGui::End();

		//パラメータ取得
		parameter.speed = speed;

		//入力されたスピード設定
		enemy->SetIdleParameter(&parameter);
	}
}
