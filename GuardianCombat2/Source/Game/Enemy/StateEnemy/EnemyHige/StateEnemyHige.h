#pragma once

class EnemyHige;

class StateEnemyHige
{
public:
	virtual void Act(EnemyHige* enemy) = 0;
};