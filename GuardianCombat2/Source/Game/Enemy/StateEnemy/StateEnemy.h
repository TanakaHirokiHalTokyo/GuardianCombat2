#pragma once

class Enemy;

class StateEnemy
{
public:
	virtual void Act(Enemy* enemy) = 0;					//�G�̊e�X�̏�Ԃ����s
	virtual void Display(Enemy* enemy) = 0;				//�p�����[�^�\��
};