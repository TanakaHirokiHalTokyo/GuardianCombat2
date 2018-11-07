#pragma once

//=====================================================
//
//		Horming����
//
//=====================================================

#include "StateAction.h"
class StateEnemyHige;
class StateEnemy;

class StateAction_Horming :public StateAction
{
public:
	StateAction_Horming(Enemy* enemy);		//�R���X�g���N�^
	~StateAction_Horming()override;			//�f�X�g���N�^
	void Action()override;					//���s�֐�
	void BeginDisplay()override;			//���O�\��
	void Display()override;					//�\��
	void EndDisplay()override;				//�\���I��

private:
	StateEnemyHige* horming_ = nullptr;			//�z�[�~���O���s
	StateEnemy* rotate_ = nullptr;				//�v���C���[�̂ق�����������
	bool rotateEnemy_ = false;
};