#pragma once

//=================================================
//
//		�z�[�~���O���s
//
//=================================================
#include "StateEnemyHige.h"

class StateEnemy_Horming :public StateEnemyHige
{
public:
	void Act(EnemyHige* enemy)override;
	void BeginDisplay(EnemyHige* enemy)override;
	void Display(EnemyHige* enemy)override;
	void EndDisplay(EnemyHige* enemy)override;
private:
	bool spawn_ = false;				//�L���[�u���X�|�[����������
	bool setPosition_ = false;			//�L���[�u��z�u������
	float transPositionValue_ = 0.0f;	//�|�W�V�����ɒB����܂łɈړ���������
};
