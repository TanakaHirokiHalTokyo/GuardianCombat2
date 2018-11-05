#pragma once

//=======================================================================
//
//		�ːi�s���@����
//
//=======================================================================

#include "StateAction.h"

class StateEnemy;
class StateEnemyHige;

class StateAction_Rush :public StateAction
{
public:
	StateAction_Rush(Enemy* enemy);			//�R���X�g���N�^
	~StateAction_Rush()override;			//�f�X�g���N�^
	void Action()override;					//���s
	void Display()override;					//�f�o�b�O�E�p�����[�^�\��
	void SetEnemy(Enemy* enemy)override;	//�G��ݒ�

private:
	StateEnemy* rotate_ = nullptr;			//�v���C���[�̂ق��ɐU�����
	StateEnemyHige* rush_ = nullptr;		//�ːi����
	Enemy* enemy_ = nullptr;				//�G�̏��
	bool isRotate_ = false;					//�v���C���[�̂ق���U���������
};