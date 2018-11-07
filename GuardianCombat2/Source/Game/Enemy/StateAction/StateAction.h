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
	virtual ~StateAction() {};								//�f�X�g���N�^
	virtual void Action() = 0;								//���s
	virtual void BeginDisplay() = 0;						//���O�\��
	virtual void Display() = 0;								//�\��
	virtual void EndDisplay() = 0;							//�\���I��
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }			//�G���Z�b�g
protected:
	Enemy* enemy_ = nullptr;
};