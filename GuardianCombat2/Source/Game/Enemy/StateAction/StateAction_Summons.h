#pragma once
//=============================================================================
//
//			���g��������
//
//=============================================================================

#include "StateAction.h"
class StateEnemy;
class StateEnemyHige;
class StateAction_Summons :public StateAction
{
public:
	StateAction_Summons(Enemy* enemy);				//�R���X�g���N�^
	~StateAction_Summons();										//�f�X�g���N�^
	void Action()override;												//���s
	void BeginDisplay()override;									//���O�\��			
	void Display()override;											//�\��
	void EndDisplay()override;										//�I���\��
private:
	bool rotate_ = false;												//�^�񒆂���������
	StateEnemy* rotateMiddle_ = nullptr;					//�^�񒆂Ɍ������鐧��
	StateEnemy* transMiddle_ = nullptr;						//�^�񒆂Ɉړ������鐧��
	StateEnemyHige* summons_ = nullptr;					//��������
};