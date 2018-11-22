#pragma once

//======================================================================
//
//			���g����
//
//======================================================================

#include "StateEnemyHige.h"
class StateEnemy_Summons :public StateEnemyHige
{
public:
	StateEnemy_Summons();													//�R���X�g���N�^
	~StateEnemy_Summons();													//�f�X�g���N�^
	void Act(EnemyHige* enemy)override;								//���s
	void BeginDisplay(EnemyHige* enemy)override;				//���O�\��
	void Display(EnemyHige* enemy)override;							//�\��
	void EndDisplay(EnemyHige* enemy)override;					//�\���I��

private:
	EnemyHige* enemy_ = nullptr;						//�G�̃|�C���^
	bool init_ = false;											//�ړ�������ݒ肵����
	float transValue_ = 0.0f;									//�ړ��������v��
	bool completed_ = false;
};