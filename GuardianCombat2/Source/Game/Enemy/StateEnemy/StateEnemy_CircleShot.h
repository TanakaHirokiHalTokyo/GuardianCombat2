#pragma once

//============================================================
//
//		�g��U��
//
//============================================================

#include "StateEnemyHige.h"
#include <d3d9.h>
#include <d3dx9.h>

class Cube;

class StateEnemy_CircleShot :public StateEnemyHige
{
public:
	void Act(EnemyHige* enemy)override;				//���s
	void BeginDisplay(EnemyHige* enemy)override;	//���O�\��
	void Display(EnemyHige* enemy)override;			//�\��
	void EndDisplay(EnemyHige* enemy)override;		//�\���I��

private:
	bool spawn_ = false;									//�X�|�[����������
	D3DXVECTOR3 *vec_ = nullptr;					//�x�N�g�����
	float length = 0.0f;										//�i�񂾋���
};