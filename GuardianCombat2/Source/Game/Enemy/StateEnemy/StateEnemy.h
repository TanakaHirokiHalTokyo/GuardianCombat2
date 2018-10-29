#pragma once

class Enemy;

class StateEnemy
{
public:
	virtual void Act(Enemy* enemy) = 0;		//“G‚ÌŠeX‚Ìó‘Ô‚ğÀs
};