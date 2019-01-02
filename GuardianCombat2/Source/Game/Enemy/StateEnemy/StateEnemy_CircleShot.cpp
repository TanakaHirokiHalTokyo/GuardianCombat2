#include "StateEnemy_CircleShot.h"
#include <d3dx9.h>
#include "../Enemy_Hige.h"
#include "../../Cube/Cube.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Imgui/ImguiManager.h"
#include "../../Effect/Effect.h"

void StateEnemy_CircleShot::Act(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	if (!spawn_)
	{
		spawn_ = true;

		//敵の位置取得
		const auto& enemy_position = enemy->GetPosition();
		//敵の前方向ベクトル取得
		const auto& enemy_front = enemy->GetVector()->GetFront();

		//角度の差分を計算
		float angle = 360.0f / (float)parameter.CUBE_NUM;

		for (int i = 0; i < parameter.CUBE_NUM; i++)
		{
			//初期化
			parameter.cube[i].Init();
			//キューブコリジョン有効化
			parameter.cube[i].GetCollision()->enable_ = true;

			//サイズ設定
			parameter.cube[i].SetScale(parameter.cubeSize,parameter.cubeSize,parameter.cubeSize);

			//キューブ可視化
			parameter.cube[i].SetVisible(true);
			parameter.effect[i].Init();

			//キューブの位置を設定
			parameter.cube[i].SetPosition(enemy_position);
			parameter.cube[i].SetPositionY(enemy_position.y + 0.5f);
			parameter.cube[i].GetCollision()->m_Pos = parameter.cube[i].GetPosition();
			parameter.effect[i].CreateEffect(parameter.cube[i].GetPosition());

			D3DXVECTOR3 vector;
			vector = parameter.vec[i].vector;

			//ベクトル算出
			D3DXMATRIX mtxRotate;
			D3DXMatrixRotationY(&mtxRotate, D3DXToRadian((float)angle * i));
			D3DXVec3TransformNormal(&vector, &enemy_front, &mtxRotate);
			D3DXVec3Normalize(&vector, &vector);

			parameter.vec[i].vector = vector;
		}
	}
	else
	{
		for (int i = 0; i < parameter.CUBE_NUM; i++)
		{
			//キューブの座標更新
			parameter.cube[i].SetPosition(parameter.cube[i].GetPosition() + parameter.speed * parameter.vec[i].vector);
			parameter.cube[i].GetCollision()->m_Pos = parameter.cube[i].GetPosition();
			parameter.cube[i].SetRotationZ(parameter.cube[i].GetRotate().z + 5.0f);
			parameter.cube[i].SetRotationX(parameter.cube[i].GetRotate().x + 5.0f);
			parameter.cube[i].SetRotationY(parameter.cube[i].GetRotate().y + 5.0f);
			parameter.cube[i].SetRotation(parameter.cube[i].GetRotate());
			parameter.effect[i].CreateEffect(parameter.cube[i].GetPosition());
			parameter.effect[i].SetVisible(true);
			parameter.effect[i].Update();
		}
		//進んだ距離更新
		length += parameter.speed;

		//スピード更新
		parameter.speed = parameter.speed + parameter.acceleration;

		//距離が一定以上だった場合行動終了
		if (length >= parameter.length)
		{
			for (int i = 0; i < parameter.CUBE_NUM; i++)
			{
				parameter.effect[i].SetVisible(false);
				parameter.cube[i].SetVisible(false);
				parameter.speed = parameter.inital_velocity;
				parameter.effect[i].Uninit();
				parameter.cube[i].GetCollision()->enable_ = false;
			}
			enemy->FinishState();
		}
	}
	enemy->SetCircleShotParameter(&parameter);
}

void StateEnemy_CircleShot::BeginDisplay(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].BeginDraw();
	}
}

void StateEnemy_CircleShot::Display(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].Draw();
		parameter.effect[i].Draw();
	}
}

void StateEnemy_CircleShot::EndDisplay(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].EndDraw();
	}
}
