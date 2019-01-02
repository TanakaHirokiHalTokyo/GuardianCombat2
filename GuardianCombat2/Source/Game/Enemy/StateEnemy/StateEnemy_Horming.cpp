#include "StateEnemy_Horming.h"
#include "../Enemy_Hige.h"
#include "../../../Vector3/Vector3.h"
#include "../../Cube/Cube.h"
#include "../../Player/Player.h"
#include "../../GameManager/GameManager.h"
#include "../../Effect/Effect.h"

void StateEnemy_Horming::Act(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeHorming::ENEMY_PARAMETER parameter = enemy->GetHormingParameter();
	//プレイヤー情報取得
	Player* player = GameManager::GetPlayer();
	//敵のベクトル取得
	const auto& enemy_vector = enemy->GetVector();

	//まだキューブがスポーンしていないとき
	if (!spawn_)
	{
		float spawnAngle = (float)parameter.fanangle / (parameter.CUBE_NUM - 1);
		float initAngle = (180.0f - parameter.fanangle) / 2.0f;

		for (int i = 0; i < parameter.CUBE_NUM; i++)
		{
			parameter.cube[i].Init();

			parameter.cube[i].SetScale(parameter.cubeSize, parameter.cubeSize, parameter.cubeSize);

			D3DXVECTOR3 leftVector = enemy_vector->GetRight();
			leftVector *= -1.0f;
			D3DXVECTOR3 frontVector = enemy_vector->GetFront();
			D3DXQUATERNION quaternion;

			parameter.spawnvec[i].vector = leftVector;

			D3DXQuaternionRotationAxis(&quaternion,&frontVector,D3DXToRadian(initAngle + (spawnAngle * i)));

			D3DXMATRIX mtxRotate;
			D3DXMatrixRotationQuaternion(&mtxRotate,&quaternion);

			D3DXVec3TransformNormal(&parameter.spawnvec[i].vector, &parameter.spawnvec[i].vector, &mtxRotate);
			D3DXVec3Normalize(&parameter.spawnvec[i].vector, &parameter.spawnvec[i].vector);

			parameter.cube[i].SetPosition(enemy->GetPosition());
			parameter.cube[i].SetVisible(true);
			
			spawn_ = true;
		}
	}
	else if (!setPosition_)
	{
		for (int i = 0; i < parameter.CUBE_NUM; i++)
		{
			parameter.cube[i].SetPosition(parameter.cube[i].GetPosition() + (parameter.spawnvec[i].vector * parameter.setposition_speed));
			parameter.cube[1].GetCollision()->m_Pos = parameter.cube[i].GetPosition();
		}
		transPositionValue_ += parameter.setposition_speed;

		if (transPositionValue_ >= parameter.radius)
		{
			setPosition_ = true;
			//エフェクト初期化
			for (int i = 0; i < parameter.CUBE_NUM; i++)
			{
				parameter.effect[i].Init();
				parameter.effect[i].SetVisible(true);
				parameter.cube[i].GetCollision()->enable_ = true;
			}
		}
	}
	else
	{
		for (int i = 0; i < parameter.CUBE_NUM; i++)
		{
			if (!parameter.shot[i])
			{
				if (i == 0)
				{
					parameter.cooltimecount[i] = 0;
					D3DXVECTOR3 front = player->GetPosition() - parameter.cube[i].GetPosition();
					D3DXVec3Normalize(&front,&front);
					parameter.vec[i].SetFront(front);

					parameter.shot[i] = true;
					parameter.alivetimecount[i] = 0;
				}
				else
				{
					if (parameter.shot[i - 1] && parameter.cooltimecount[i - 1] >= parameter.cooltime)
					{
						parameter.cooltimecount[i] = 0;
						D3DXVECTOR3 front = player->GetPosition() - parameter.cube[i].GetPosition();
						D3DXVec3Normalize(&front, &front);
						parameter.vec[i].SetFront(front);

						parameter.shot[i] = true;
						parameter.alivetimecount[i] = 0;
					}
				}
			}
			else
			{
				parameter.alivetimecount[i]++;
				parameter.cooltimecount[i]++;

				//ホーミングさせるためにベクトルを少しずつプレイヤーのほうに回転させる
				D3DXVECTOR3 shotRight;
				D3DXVec3Cross(&shotRight, &parameter.vec[i].GetUp(), &parameter.vec[i].GetFront());
				D3DXVec3Normalize(&shotRight, &shotRight);
				parameter.vec[i].SetRight(shotRight);

				D3DXVECTOR3 hormingVector = player->GetPosition() - parameter.cube[i].GetPosition();
				D3DXVec3Normalize(&hormingVector, &hormingVector);

				float Dot = D3DXVec3Dot(&hormingVector, &parameter.vec[i].GetFront());
				float RightDot = D3DXVec3Dot(&parameter.vec[i].GetRight(), &hormingVector);
				float Angle = D3DXToDegree(acosf(Dot));
				if (isnan(Angle)) {
					Angle = 0.0f;
				}

				if (RightDot >= 0.0f)
				{
					//回転行列作成
					D3DXMATRIX mtxRotate;
					D3DXVECTOR3 frontVec = parameter.vec[i].GetFront();
					D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(parameter.horming_accuracy));
					D3DXVec3TransformNormal(&frontVec, &parameter.vec[i].GetFront(), &mtxRotate);
					D3DXVec3Normalize(&frontVec, &frontVec);
					parameter.vec[i].SetFront(frontVec);
				}
				else
				{
					//回転行列作成
					D3DXMATRIX mtxRotate;
					D3DXVECTOR3 frontVec = parameter.vec[i].GetFront();
					D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(-parameter.horming_accuracy));
					D3DXVec3TransformNormal(&frontVec, &parameter.vec[i].GetFront(), &mtxRotate);
					D3DXVec3Normalize(&frontVec, &frontVec);
					parameter.vec[i].SetFront(frontVec);
				}

				parameter.cube[i].SetPosition(parameter.cube[i].GetPosition() + (parameter.vec[i].GetFront() * parameter.speed[i]));
				parameter.cube[i].GetCollision()->m_Pos = parameter.cube[i].GetPosition();

				//エフェクト更新
				parameter.effect[i].CreateEffect(
					parameter.cube[i].GetPosition() + parameter.vec[i].GetFront() * parameter.speed[i]);

				parameter.speed[i] += parameter.acceleration;

				if (parameter.alivetimecount[i] >= parameter.alivetime)
				{
					parameter.cube[i].SetVisible(false);
					if (i == parameter.CUBE_NUM - 1)
					{
						for (int i = 0; i < parameter.CUBE_NUM; i++)
						{
							parameter.alivetimecount[i] = 0;
							parameter.cooltimecount[i] = 0;
							parameter.shot[i] = false;
							parameter.speed[i] = parameter.inital_velocity;
							spawn_ = false;
							setPosition_ = false;
							parameter.cube[i].GetCollision()->enable_ = false;

							//エフェクト終了処理
							parameter.effect[i].Uninit();
							parameter.effect[i].SetVisible(false);
						}
						enemy->FinishState();
					}
				}
			}
		}
	}
	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].SetRotation(parameter.cube[i].GetRotate().x + 5.0f, parameter.cube[i].GetRotate().y + 5.0f, parameter.cube[i].GetRotate().z + 5.0f);
		parameter.effect[i].Update();
	}

	enemy->SetHormingParameter(&parameter);
}

void StateEnemy_Horming::BeginDisplay(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeHorming::ENEMY_PARAMETER parameter = enemy->GetHormingParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].BeginDraw();
	}
}

void StateEnemy_Horming::Display(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeHorming::ENEMY_PARAMETER parameter = enemy->GetHormingParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].Draw();
		parameter.effect[i].Draw();
	}
}

void StateEnemy_Horming::EndDisplay(EnemyHige * enemy)
{
	//パラメータ取得
	EnemyHigeHorming::ENEMY_PARAMETER parameter = enemy->GetHormingParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].EndDraw();
	}
}
