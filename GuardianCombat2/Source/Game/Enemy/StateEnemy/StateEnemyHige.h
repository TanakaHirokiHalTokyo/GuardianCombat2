#pragma once

//==========================================================
//
//		�G�i�q�Q�j�̍s������
//
//==========================================================

class EnemyHige;

class StateEnemyHige
{
public:
	virtual void Act(EnemyHige* enemy) = 0;
	virtual void Display(EnemyHige* enemy) = 0;
};