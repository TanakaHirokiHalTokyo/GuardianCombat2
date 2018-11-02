#pragma once

//======================================================================
//
//		敵の状態パターンによる行動制御
//
//======================================================================

class Enemy;

class StatePattern_Enemy
{
public:
	virtual ~StatePattern_Enemy() {};
	virtual void Act() = 0;
	virtual void ChangeState() = 0;
};