#pragma once

#include "Enemy.h"

class StatePattern_Enemy;
class Sphere;
class DebugSphere;
class Cube;
class AdditionEffect;

class ParameterVector
{
public:
	D3DXVECTOR3 vector;
};

namespace EnemyHigeRush				//�ːi��Ԃ̃p�����[�^
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.5f;						//�X�s�[�h
		float length = 10.0f;						//�ːi����
	};
}
namespace EnemyHigeCircleShot					//�g��U���̏��
{
	struct ENEMY_PARAMETER
	{
		int CUBE_NUM = 10;								//�L���[�u�̐�
		int OLD_CUBE_NUM = 10;						//�O��̃L���[�u�̐�
		float inital_velocity = 0.1f;						//�L���[�u�̔��ł��������x
		float acceleration = 0.001f;						//�����x
		float speed = inital_velocity;					//���݂̃X�s�[�h
		float length = 10.0f;									//�L���[�u���΂�����
		float cubeSize = 1.0f;								//�L���[�u�̃T�C�Y
		Cube* cube = nullptr;								//�L���[�u���
		ParameterVector* vec = nullptr;				//�x�N�g�����
		AdditionEffect* effect = nullptr;							//�G�t�F�N�g���
	};
}

namespace EnemyHigeHorming
{
	struct ENEMY_PARAMETER
	{
		int CUBE_NUM = 3;											//�L���[�u�̐�
		int OLD_CUBE_NUM = CUBE_NUM;					//�O��̃L���[�u�̐�
		float inital_velocity = 0.1f;									//�L���[�u�̔��ł��������x
		float acceleration = 0.001f;									//�����x
		float* speed = nullptr;											//���݂̃X�s�[�h
		float cubeSize = 1.0f;											//�L���[�u�̃T�C�Y
		float fanangle = 120.0f;										//�L���[�u�X�|�[�����̐��p�x
		float setposition_speed = 0.1f;							//�|�W�V�����Ɍ������X�s�[�h
		float radius = 5.0f;												//��̔��a
		int cooltime = 30;												//���̒e��ł܂ł̃t���[����
		int* cooltimecount = nullptr;								//�N�[���^�C���J�E���g
		bool* shot = nullptr;											//���݂̃C���f�b�N�X�ԍ��̒e��ł��Ă���̂�
		int alivetime = 120;												//��������
		int* alivetimecount = nullptr;								//�������ԃJ�E���g
		float horming_accuracy = 0.2f;							//�z�[�~���O���x
		Cube* cube = nullptr;											//�L���[�u���
		Vector3* vec = nullptr;										//�x�N�g�����
		ParameterVector* spawnvec = nullptr;				//�X�|�[�����̃x�N�g�����
		AdditionEffect* effect = nullptr;							//�G�t�F�N�g���
	};
}


class EnemyHige :public Enemy
{
public:
	enum STATE
	{
		IDLE,						//�ҋ@���
		RUSH,						//�ːi
		CIRCLESHOT,			//�g��U��
		HORMING,				//�z�[�~���O�e
		STATE_MAX,
	};
	const char* StateWord[STATE_MAX]
	{
		"IDLE",
		"RUSH",
		"CIRCLESHOT",
		"HORMING",
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
	void SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER* parameter);

	//�g��U���̃p�����[�^�擾�E�ݒ�
	EnemyHigeCircleShot::ENEMY_PARAMETER GetCircleShotParameter();
	void SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER* parameter);

	//�z�[�~���O�̃p�����[�^�擾�E�ݒ�
	EnemyHigeHorming::ENEMY_PARAMETER GetHormingParameter();
	void SetHormingParameter(EnemyHigeHorming::ENEMY_PARAMETER* parameter);

private:
	void DrawDebug();															//Debug�\��
	void InitParameter();														//�p�����[�^������
	void ReCreateCircleParameter();									//�g��U���p�����[�^�č쐬
	void ReCreateHormingParameter();								//�z�[�~���O�p�����[�^�č쐬
	void InitCircleParameterValue();										//�g��p�����[�^������
	void InitHormingParameterValue();								//�p�����[�^�̏ڍׂ�������
	void DestParameter();														//�p�����[�^�f�X�g���N�^
private:
	StatePattern_Enemy* statePattern_ = nullptr;				//�X�e�[�g��ԊǗ�
	STATE state_ = IDLE;														//���
	Sphere* collision_ = nullptr;											//�R���W�������

	EnemyHigeRush::ENEMY_PARAMETER rushParameter_ = {};							//�ːi��Ԃ̃p�����[�^���
	EnemyHigeCircleShot::ENEMY_PARAMETER circleShotParameter_ = {};		//�g��U���̃p�����[�^���
	EnemyHigeHorming::ENEMY_PARAMETER hormingParameter_ = {};				//�z�[�~���O�̃p�����[�^���
	XModel* ring_ = nullptr;
};