#pragma once

//================================================
//
//		�G�i�q�Q�j�̏�ԕω��ɂ��s������
//
//================================================

#include "StatePattern_Enemy.h"

class StateAction;
class EnemyHige;

class StatePattern_EnemyHige :public StatePattern_Enemy
{
public:
	StatePattern_EnemyHige(EnemyHige * enemy);		//�R���X�g���N�^
	~StatePattern_EnemyHige()override;				//�f�X�g���N�^
	void Act()override;								//���s
	void ChangeState()override;						//��ԕύX

	void SetEnemyHige(EnemyHige* enemy);			//�G��ݒ�
private:
	StatePattern_EnemyHige(){}						//��������R���X�g���N�^�������ĂԂ悤��

	EnemyHige* enemy_ = nullptr;					//�G�̏��
	StateAction* action_ = nullptr;					//���s���
};