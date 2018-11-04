#pragma once

#include "Enemy.h"

class StatePattern_Enemy;
class Sphere;
class DebugSphere;

class EnemyHige :public Enemy
{
public:
	enum STATE
	{
		IDLE,
		STATE_MAX,
	};
	const std::string StateWord[STATE_MAX]
	{
		"IDLE",
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

	

private:
	void DrawDebug();								//Debug�\��
private:
	StatePattern_Enemy* statePattern_ = nullptr;	//�X�e�[�g��ԊǗ�
	STATE state_ = IDLE;							//���
	Sphere* collision_ = nullptr;					//�R���W�������
	DebugSphere* debug;
};