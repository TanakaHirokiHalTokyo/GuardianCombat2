#pragma once

//======================================================
//
//		��Ԃ����ۂɎ��s����
//
//======================================================

class Enemy;

class StateAction
{
public:
	virtual ~StateAction() {};					//�f�X�g���N�^
	virtual void Action() = 0;					//���s
	virtual void SetEnemy(Enemy* enemy) = 0;	//�G���Z�b�g
};