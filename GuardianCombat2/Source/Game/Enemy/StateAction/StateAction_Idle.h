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
	void BeginDisplay()override;			//���O�\��
	void Display()override;					//�\��
	void EndDisplay()override;				//�\���I��
private:
	bool IsApproach();							//����ȏ�v���C���[�ɋ߂Â����𔻒f

	StateEnemy* move_ = nullptr;			//�ړ��𐧌�
	StateEnemy* rotate_ = nullptr;			//��]��񐧌�
};