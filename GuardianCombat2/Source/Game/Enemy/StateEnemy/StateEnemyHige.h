#pragma once

//==========================================================
//
//		“GiƒqƒQj‚Ìs“®§Œä
//
//==========================================================

class EnemyHige;

class StateEnemyHige
{
public:
	virtual void Act(EnemyHige* enemy) = 0;
	virtual void Display(EnemyHige* enemy) = 0;
};