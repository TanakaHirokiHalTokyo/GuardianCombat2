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
	virtual ~StatePattern_Enemy() {};		//�f�X�g���N�^
	virtual void Act() = 0;					//���s
	virtual void Display() = 0;				//�\��
	virtual void ChangeState() = 0;			//�X�e�[�g�ύX
};