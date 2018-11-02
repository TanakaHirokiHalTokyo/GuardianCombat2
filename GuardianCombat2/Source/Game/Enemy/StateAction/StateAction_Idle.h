#pragma once

//==================================================
//
//		�ҋ@��Ԃ����s
//
//==================================================

#include "StateAction.h"
class StateEnemy;

class StateAction_Idle :public StateAction
{
public:
	StateAction_Idle(Enemy* enemy);			//�R���X�g���N�^
	~StateAction_Idle()override;			//�f�X�g���N�^
	void Action()override;					//���s
	void Display()override;					//�\��
	void SetEnemy(Enemy* enemy)override;	//�G��ݒ�
private:
	StateEnemy* move_ = nullptr;			//�ړ��𐧌�
	StateEnemy* rotate_ = nullptr;			//��]��񐧌�
	Enemy* enemy_ = nullptr;				//�G�̏��
};