#pragma once
//============================================================
//
//		�e���|�[�g
//
//============================================================
#include "StateEnemyHige.h"
#include <d3dx9.h>

class Player;

class StateEnemy_Teleportation :public StateEnemyHige
{
public:
	const float DIVE_SPEED = 0.01f;

	void Act(EnemyHige* enemy)override;
	void BeginDisplay(EnemyHige* enemy)override;
	void Display(EnemyHige* enemy)override;
	void EndDisplay(EnemyHige* enemy)override;
private:
	bool setVector_ = false;																		//�x�N�g����ݒ肵����
	bool playEffect_ = false;																		//�G�t�F�N�g���Đ�������
	float diveValue_ = 1.0f;																		//�����
	float divingValue_ = 0.0f;																	//�����Ă����
	D3DXVECTOR3 teleportVector_ = D3DXVECTOR3(0,0,0);				//�v���C���[�̈ʒu����݂ăe���|�[�g�̌���
	Player* player_ = nullptr;																	//Player���
};