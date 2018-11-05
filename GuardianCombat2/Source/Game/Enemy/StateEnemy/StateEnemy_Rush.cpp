#include "StateEnemy_Rush.h"
#include "../Enemy_Hige.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/Player.h"
#include "../Enemy.h"
#include "../../../Imgui/ImguiManager.h"
#include "../../../Vector3/Vector3.h"

void StateEnemy_Rush::Act(EnemyHige * enemy)
{
	//Player���擾
	const auto& player_info = GameManager::GetPlayer();

	//�p�����[�^�擾
	EnemyHigeRush::ENEMY_PARAMETER parameter = enemy->GetRushParameter();
	
	//���W�X�V
	enemy->SetPosition(enemy->GetPosition() + enemy->GetVector()->GetFront() * parameter.speed);

	//�i�񂾋����X�V
	rushLength_ += parameter.speed;

	//�ݒ肵���������ːi������ːi�I��
	if (rushLength_ > parameter.length)
	{
		//�s���I���@IDLE��Ԃ�
		enemy->FinishState();
	}
}

void StateEnemy_Rush::Display(EnemyHige * enemy)
{
	//�f�o�b�O���[�h��
	if (enemy->GetDebugMode())
	{
		//�p�����[�^�擾
		EnemyHigeRush::ENEMY_PARAMETER parameter = enemy->GetRushParameter();

		//�p�����[�^�ݒ�
		ImGui::Begin("EnemyDebugParameter");
		ImGui::DragFloat("Speed Set",&parameter.speed,0.01f,0.0f,1.0f);
		ImGui::DragFloat("Length Set",&parameter.length,1.0f,0.0f,100.0f);
		ImGui::End();

		//�p�����[�^�ݒ�
		enemy->SetRushParameter(parameter);
	}
}