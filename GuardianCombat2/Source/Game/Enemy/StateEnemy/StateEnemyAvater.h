#pragma once

class EnemyHige_Avater;
class StateEnemyAvater
{
public:
	virtual void Action(EnemyHige_Avater* enemy) = 0;
	virtual void BeginDisplay(EnemyHige_Avater* enemy) = 0;
	virtual void Display(EnemyHige_Avater* enemy) = 0;
	virtual void EndDisplay(EnemyHige_Avater* enemy) = 0;
};