#pragma once

class Enemy;

class StateEnemy
{
public:
	virtual void Act(Enemy* enemy) = 0;					//敵の各々の状態を実行
	virtual void Display(Enemy* enemy) = 0;				//パラメータ表示
};