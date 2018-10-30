#include "StateEnemy_Rotate.h"
#include "../Enemy.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/Player.h"
#include "../../../Vector3/Vector3.h"
#include "../../XModel/XModel.h"
#include "../../../Imgui/ImguiManager.h"

void StateEnemy_Rotate::Act(Enemy * enemy)
{
	//Player情報取得
	const auto& player_info = GameManager::GetPlayer();

	//EnemyModel取得
	auto enemy_model = enemy->GetModel();

	//--------------------------
	//	Playerのほうを向かせる
	//--------------------------

	//	プレイヤー方向ベクトル計算
	D3DXVECTOR3 vec_player = player_info->GetPosition() - enemy->GetPosition();
	vec_player.y = 0.0f;
	D3DXVec3Normalize(&vec_player,&vec_player);

	//ベクトル取得
	const auto& enemy_vector = enemy->GetVector();

	//　前方向ベクトル取得
	D3DXVECTOR3 front = enemy_vector->GetFront();
	//	右方向ベクトル取得
	D3DXVECTOR3 right = enemy_vector->GetRight();
	//	上方向ベクトル取得
	D3DXVECTOR3 up = enemy_vector->GetUp();

	//内積計算
	float Dot = D3DXVec3Dot(&front, &vec_player);			//プレイヤーと敵の差角(ラジアン角度)
	float RightDot = D3DXVec3Dot(&right, &vec_player);		//右回りか左回りか
	float Angle = D3DXToDegree(acosf(Dot));
	if (isnan(Angle)) {
		Angle = 0.0f;
	}

	if (RightDot >= 0.0f)
	{
		enemy->SetRotationY(enemy->GetRotate().y - Angle);
		enemy_model->SetRotation(enemy->GetRotate());

		//回転行列を作成
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

		//回転行列を作成
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
