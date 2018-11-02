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

void StateEnemy_MoveToPlayer::Display(Enemy* enemy)
{
	//�f�o�b�O���[�h��
	if (enemy->GetDebugMode())
	{
		//�G�̃X�s�[�h�擾
		float speed =enemy->GetSpeed();

		//�p�����[�^�\��
		ImGui::Begin("EnemyDebugParameter");
		ImGui::DragFloat("Speed Set", &speed, 0.001f, 0.0f, 0.1f);
		ImGui::End();

		//���͂��ꂽ�X�s�[�h�ݒ�
		enemy->SetSpeed(speed);
	}
}
