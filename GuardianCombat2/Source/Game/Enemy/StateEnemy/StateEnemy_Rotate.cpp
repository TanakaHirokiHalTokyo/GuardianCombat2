#include "StateEnemy_Rotate.h"
#include "../Enemy.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/Player.h"
#include "../../../Vector3/Vector3.h"
#include "../../XModel/XModel.h"
#include "../../../Imgui/ImguiManager.h"

void StateEnemy_Rotate::Act(Enemy * enemy)
{
	//Player���擾
	const auto& player_info = GameManager::GetPlayer();

	//EnemyModel�擾
	auto enemy_model = enemy->GetModel();

	//--------------------------
	//	Player�̂ق�����������
	//--------------------------

	//	�v���C���[�����x�N�g���v�Z
	D3DXVECTOR3 vec_player = player_info->GetPosition() - enemy->GetPosition();
	vec_player.y = 0.0f;
	D3DXVec3Normalize(&vec_player,&vec_player);

	//�x�N�g���擾
	const auto& enemy_vector = enemy->GetVector();

	//�@�O�����x�N�g���擾
	D3DXVECTOR3 front = enemy_vector->GetFront();
	//	�E�����x�N�g���擾
	D3DXVECTOR3 right = enemy_vector->GetRight();
	//	������x�N�g���擾
	D3DXVECTOR3 up = enemy_vector->GetUp();

	//���όv�Z
	float Dot = D3DXVec3Dot(&front, &vec_player);			//�v���C���[�ƓG�̍��p(���W�A���p�x)
	float RightDot = D3DXVec3Dot(&right, &vec_player);		//�E��肩����肩
	float Angle = D3DXToDegree(acosf(Dot));
	if (isnan(Angle)) {
		Angle = 0.0f;
	}

	if (RightDot >= 0.0f)
	{
		enemy->SetRotationY(enemy->GetRotate().y - Angle);
		enemy_model->SetRotation(enemy->GetRotate());

		//��]�s����쐬
		D3DXMATRIX mtxRotate;
		D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(-Angle));
		D3DXVec3TransformNormal(&front, &front, &mtxRotate);
		D3DXVec3Normalize(&front, &front);
		enemy_vector->SetFront(front);
		D3DXVec3Cross(&right, &enemy_vector->GetFront(), &enemy_vector->GetUp());
		D3DXVec3Normalize(&right, &right);
		enemy_vector->SetRight(right);
	}
	else
	{
		enemy->SetRotationY(enemy->GetRotate().y + Angle);
		enemy_model->SetRotation(enemy->GetRotate());

		//��]�s����쐬
		D3DXMATRIX mtxRotate;
		D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(Angle));
		D3DXVec3TransformNormal(&front, &front, &mtxRotate);
		D3DXVec3Normalize(&front, &front);
		enemy_vector->SetFront(front);
		D3DXVec3Cross(&right, &enemy_vector->GetFront(), &enemy_vector->GetUp());
		D3DXVec3Normalize(&right, &right);
		enemy_vector->SetRight(right);
	}

	ImGui::Begin("Angle");
	ImGui::Text("Angle : %f",Angle);
	ImGui::Text("Rotate : %f %f %f",enemy_model->GetRotate().x, enemy_model->GetRotate().y, enemy_model->GetRotate().z);
	ImGui::End();
}
