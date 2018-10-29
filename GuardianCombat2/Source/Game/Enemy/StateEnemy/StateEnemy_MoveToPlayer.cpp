#include "StateEnemy_MoveToPlayer.h"
#include "../Enemy.h"
#include "../../../Vector3/Vector3.h"

void StateEnemy_MoveToPlayer::Act(Enemy * enemy)
{
	//�G�̃x�N�g���擾
	const auto& enemy_vector = enemy->GetVector();

	enemy->SetPosition(enemy->GetPosition() + enemy_vector->GetFront() * enemy->GetSpeed());
}
