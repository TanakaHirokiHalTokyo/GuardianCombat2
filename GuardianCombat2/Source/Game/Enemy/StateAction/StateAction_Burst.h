#pragma once

//=====================================================
//
//			�o�[�X�g�V���b�g�̐���
//
//=======================================================

#include "StateAction.h"

class StateEnemy;
class StateEnemyHige;

class StateAction_Burst :public StateAction
{
public:
	StateAction_Burst(Enemy* enemy);
	~StateAction_Burst()override;
	void Action()override;
	void BeginDisplay()override;
	void Display()override;
	void EndDisplay()override;

private:
	bool alreadyrot_ = false;								//���łɐU������Ă���̂�
	StateEnemy* rotate_ = nullptr;					//��]���s
	StateEnemyHige* burst_ = nullptr;				//�o�[�X�g���s
};