#pragma once

#include "Enemy.h"

class StatePattern_Enemy;
class Sphere;
class DebugSphere;
class Cube;

class CircleVector
{
public:
	D3DXVECTOR3 vector;
};

namespace EnemyHigeRush							//�ːi��Ԃ̃p�����[�^
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.5f;						//�X�s�[�h
		float length = 10.0f;					//�ːi����
	};
}
namespace EnemyHigeCircleShot					//�g��U���̏��
{
	struct ENEMY_PARAMETER
	{
		int CUBE_NUM = 10;						//�g��U�����̃L���[�u�̐�
		int OLD_CUBE_NUM = 10;					//�O��̃L���[�u�̐�
		float inital_velocity = 0.1f;			//�L���[�u�̔��ł��������x
		float acceleration = 0.001f;			//�����x
		float speed = inital_velocity;			//���݂̃X�s�[�h
		float length = 10.0f;					//�L���[�u���΂�����
		float cubeSize = 1.0f;					//�L���[�u�̃T�C�Y
		Cube* cube = nullptr;					//�L���[�u���
		CircleVector* vec = nullptr;			//�x�N�g�����
	};
}


class EnemyHige :public Enemy
{
public:
	enum STATE
	{
		IDLE,					//�ҋ@���
		RUSH,					//�ːi
		CIRCLESHOT,				//�g��U��
		STATE_MAX,
	};
	const char* StateWord[STATE_MAX]
	{
		"IDLE",
		"RUSH",
		"CIRCLESHOT",
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

	//�g��U���̃p�����[�^�擾�E�ݒ�
	EnemyHigeCircleShot::ENEMY_PARAMETER GetCircleShotParameter();
	void SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER parameter);

private:
	void DrawDebug();													//Debug�\��
private:
	StatePattern_Enemy* statePattern_ = nullptr;						//�X�e�[�g��ԊǗ�
	STATE state_ = IDLE;												//���
	Sphere* collision_ = nullptr;										//�R���W�������
	DebugSphere* debugCollision_ = nullptr;								//�R���W�������������邩

	EnemyHigeRush::ENEMY_PARAMETER rushParameter_ = {};					//�ːi��Ԃ̃p�����[�^���
	EnemyHigeCircleShot::ENEMY_PARAMETER circleShotParameter_ = {};		//�g��U���̃p�����[�^���
};