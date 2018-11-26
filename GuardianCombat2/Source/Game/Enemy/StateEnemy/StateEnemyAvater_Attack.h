#pragma once

#include "StateEnemyAvater.h"
#include <d3dx9.h>

class Cube;
class StateEnemyAvater_Attack :public StateEnemyAvater
{
public:
	void Action(EnemyHige_Avater* enemy)override;
	void BeginDisplay(EnemyHige_Avater* enemy)override;
	void Display(EnemyHige_Avater* enemy)override;
	void EndDisplay(EnemyHige_Avater* enemy)override;
private:
	Cube* cube_ = nullptr;
	bool init_ = false;
	int counter_ = 0;
	bool shooting_ = false;
	int shootingcounter_ = 0;
	D3DXVECTOR3 shotVector_ = D3DXVECTOR3(0,0,0);
};