#include "StateEnemy_MoveToPlayer.h"
#include "../Enemy.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Imgui/ImguiManager.h"

void StateEnemy_MoveToPlayer::Act(Enemy * enemy)
{
	//�G�̃x�N�g���擾
	const auto& enemy_vector = enemy->GetVector();

	//���W��ݒ�
	enemy->SetPosition(enemy->GetPosition() + enemy_vector->GetFront() * enemy->GetSpeed());
}

void StateEnemy_MoveToPlayer::BeginDisplay(Enemy * enemy)
{
}

void StateEnemy_MoveToPlayer::Display(Enemy* enemy)
{
	
}

void StateEnemy_MoveToPlayer::EndDisplay(Enemy * enemy)
{
}
