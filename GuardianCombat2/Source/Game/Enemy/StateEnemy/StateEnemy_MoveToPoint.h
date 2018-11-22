#pragma once

//===================================================================
//
//			�w�肵���n�_�Ɉړ�������
//
//===================================================================
#include "StateEnemy.h"
#include <d3dx9.h>

class StateEnemy_MoveToPoint :public StateEnemy
{
public:
	StateEnemy_MoveToPoint(D3DXVECTOR3 point ,float speed);	//�R���X�g���N�^
	~StateEnemy_MoveToPoint();														//�f�X�g���N�^
	void Act(Enemy* enemy)override;													//���s
	void BeginDisplay(Enemy* enemy)override;									//���O�\��
	void Display(Enemy* enemy)override;											//�\��
	void EndDisplay(Enemy* enemy)override;										//�I���\��

	bool GetArrial();
private:
	D3DXVECTOR3 point_ = D3DXVECTOR3(0,0,0);		//�ړ���
	bool askLength_ = false;												//�ړ���܂ł̋��������߂����B
	float length_ = 0.0f;														//�ړ���܂ł̋���
	float speed_ = 0.0f;														//�ړ�����X�s�[�h
	float transValue_ = 0.0f;												//�ړ��������v��
	bool arrival_ = false;													//�ړ���ɓ��B������
};