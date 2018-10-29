#pragma once

#include "Enemy.h"

class StateEnemy;

class EnemyHige :public Enemy
{
public:
	EnemyHige();
	~EnemyHige();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	const float DEFAULT_SPEED = 0.1f;			//�f�t�H���g�X�s�[�h
private:
	StateEnemy* rotate_ = nullptr;				//��]����
	StateEnemy* moveToPlayer_ = nullptr;		//�v���C���[�Ɍ���������
};