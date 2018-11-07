#include "StateEnemy_CircleShot.h"
#include <d3dx9.h>
#include "../Enemy_Hige.h"
#include "../../Cube/Cube.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Imgui/ImguiManager.h"

void StateEnemy_CircleShot::Act(EnemyHige * enemy)
{
	//�p�����[�^�擾
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	if (!spawn_)
	{
		spawn_ = true;

		//�G�̈ʒu�擾
		const auto& enemy_position = enemy->GetPosition();
		//�G�̑O�����x�N�g���擾
		const auto& enemy_front = enemy->GetVector()->GetFront();

		//�p�x�̍������v�Z
		float angle = 360.0f / (float)parameter.CUBE_NUM;

		for (int i = 0; i < parameter.CUBE_NUM; i++)
		{
			//������
			parameter.cube[i].Init();

			//�T�C�Y�ݒ�
			parameter.cube[i].SetScale(parameter.cubeSize,parameter.cubeSize,parameter.cubeSize);

			//�L���[�u����
			parameter.cube[i].SetVisible(true);

			//�L���[�u�̈ʒu��ݒ�
			parameter.cube[i].SetPosition(enemy_position);
			parameter.cube[i].SetPositionY(enemy_position.y + 0.5f);

			D3DXVECTOR3 vector;
			vector = parameter.vec[i].vector;

			//�x�N�g���Z�o
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
			//�L���[�u�̍��W�X�V
			parameter.cube[i].SetPosition(parameter.cube[i].GetPosition() + parameter.speed * parameter.vec[i].vector);
			parameter.cube[i].SetRotationZ(parameter.cube[i].GetRotate().z + 5.0f);
			parameter.cube[i].SetRotationX(parameter.cube[i].GetRotate().x + 5.0f);
			parameter.cube[i].SetRotationY(parameter.cube[i].GetRotate().y + 5.0f);
			parameter.cube[i].SetRotation(parameter.cube[i].GetRotate());
		}
		//�i�񂾋����X�V
		length += parameter.speed;

		//�X�s�[�h�X�V
		parameter.speed = parameter.speed + parameter.acceleration;

		//���������ȏゾ�����ꍇ�s���I��
		if (length >= parameter.length)
		{
			for (int i = 0; i < parameter.CUBE_NUM; i++)
			{
				parameter.cube[i].SetVisible(false);
				parameter.speed = parameter.inital_velocity;
			}
			enemy->FinishState();
		}
	}
	enemy->SetCircleShotParameter(&parameter);
}

void StateEnemy_CircleShot::BeginDisplay(EnemyHige * enemy)
{
	//�p�����[�^�擾
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].BeginDraw();
	}
}

void StateEnemy_CircleShot::Display(EnemyHige * enemy)
{
	//�p�����[�^�擾
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].Draw();
	}
}

void StateEnemy_CircleShot::EndDisplay(EnemyHige * enemy)
{
	//�p�����[�^�擾
	EnemyHigeCircleShot::ENEMY_PARAMETER parameter = enemy->GetCircleShotParameter();

	for (int i = 0; i < parameter.CUBE_NUM; i++)
	{
		parameter.cube[i].EndDraw();
	}
}
