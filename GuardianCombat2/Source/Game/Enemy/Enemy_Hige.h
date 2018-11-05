#pragma once

#include "Enemy.h"

class StatePattern_Enemy;
class Sphere;
class DebugSphere;

namespace EnemyHigeRush							//�ːi��Ԃ̃p�����[�^
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.5f;						//�X�s�[�h
		float length = 10.0f;					//�ːi����
	};
}


class EnemyHige :public Enemy
{
public:
	enum STATE
	{
		IDLE,					//�ҋ@���
		RUSH,					//�ːi
		STATE_MAX,
	};
	const char* StateWord[STATE_MAX]
	{
		"IDLE",
		"RUSH",
	};
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

	STATE GetState();
	void SetState(STATE state);

	//�s�����I�����ۂ�IDLE��Ԃ�
	void FinishState();

	//�ːi��Ԃ̃p�����[�^�擾�E�ݒ�
	EnemyHigeRush::ENEMY_PARAMETER GetRushParameter();
	void SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER parameter);

private:
	void DrawDebug();										//Debug�\��
private:
	StatePattern_Enemy* statePattern_ = nullptr;			//�X�e�[�g��ԊǗ�
	STATE state_ = IDLE;									//���
	Sphere* collision_ = nullptr;							//�R���W�������
	DebugSphere* debugCollision_ = nullptr;					//�R���W�������������邩

	EnemyHigeRush::ENEMY_PARAMETER rushParameter_ = {};		//�ːi��Ԃ̃p�����[�^���
};