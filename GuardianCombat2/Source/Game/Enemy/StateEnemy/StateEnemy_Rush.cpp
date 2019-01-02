#include "StateEnemy_Rush.h"
#include "../Enemy_Hige.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/Player.h"
#include "../Enemy.h"
#include "../../../Imgui/ImguiManager.h"
#include "../../../Vector3/Vector3.h"

void StateEnemy_Rush::Act(EnemyHige * enemy)
{
	//�p�����[�^�擾
	EnemyHigeRush::ENEMY_PARAMETER parameter = enemy->GetRushParameter();

	const auto& vector = enemy->GetVector();
	enemy->SetCollisionEnable(true);
	enemy->SetCollisionDPS(parameter.attack);

	//���W�X�V
	enemy->SetPosition(enemy->GetPosition() + vector->GetFront() * parameter.speed);

	//�i�񂾋����X�V
	rushLength_ += parameter.speed;

	//�ݒ肵���������ːi������ːi�I��
	if (rushLength_ > parameter.length)
	{
		enemy->SetCollisionEnable(false);
		enemy->SetCollisionDPS(0.0f);
		//�s���I���@IDLE��Ԃ�
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
