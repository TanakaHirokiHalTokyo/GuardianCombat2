#pragma once

//======================================================================
//
//		�G�̏�ԃp�^�[���ɂ��s������
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