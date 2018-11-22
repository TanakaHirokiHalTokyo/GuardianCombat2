#include "StateEnemy_RotateToMiddle.h"
#include <d3dx9.h>
#include "../Enemy.h"
#include "../../../Vector3/Vector3.h"
#include "../../XModel/XModel.h"

const D3DXVECTOR3 middle_position = D3DXVECTOR3(0,0,0);

void StateEnemy_RotateToMiddle::Act(Enemy * enemy)
{
	//EnemyModel�擾
	auto enemy_model = enemy->GetModel();

	//	�v���C���[�����x�N�g���v�Z
	D3DXVECTOR3 vec_middle = middle_position - enemy->GetPosition();
	vec_middle.y = 0.0f;
	D3DXVec3Normalize(&vec_middle, &vec_middle);

	//�x�N�g���擾
	const auto& enemy_vector = enemy->GetVector();

	//�@�O�����x�N�g���擾
	D3DXVECTOR3 front = enemy_vector->GetFront();
	//	�E�����x�N�g���擾
	D3DXVECTOR3 right = enemy_vector->GetRight();
	//	������x�N�g���擾
	D3DXVECTOR3 up = enemy_vector->GetUp();

	//���όv�Z
	float Dot = D3DXVec3Dot(&front, &vec_middle);			//�v���C���[�ƓG�̍��p(���W�A���p�x)
	float RightDot = D3DXVec3Dot(&right, &vec_middle);		//�E��肩����肩
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
}

void StateEnemy_RotateToMiddle::BeginDisplay(Enemy * enemy)
{
}

void StateEnemy_RotateToMiddle::Display(Enemy * enemy)
{
}

void StateEnemy_RotateToMiddle::EndDisplay(Enemy * enemy)
{
}