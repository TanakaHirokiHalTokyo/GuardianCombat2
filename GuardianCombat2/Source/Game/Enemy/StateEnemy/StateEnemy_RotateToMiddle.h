#pragma once
//=================================================================
//
//			�^�񒆂��������鏈��
//
//=================================================================

#include "StateEnemy.h"

class StateEnemy_RotateToMiddle :public StateEnemy
{
public:
	void Act(Enemy* enemy)override;							//��Ԏ��s
	void BeginDisplay(Enemy* enemy)override;			//���O�\��
	void Display(Enemy* enemy)override;					//�\��
	void EndDisplay(Enemy* enemy)override;				//�I���\��
private:

};