#pragma once

#include "Enemy.h"
#include <string>

class StatePattern_Enemy;
class Sphere;
class DebugSphere;
class Cube;
class AdditionEffect;
class CEffekseer;
class Texture;
class EnemyHige_Avater;
class OBB;
class EnemyHigeUI;

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
		float attack = 0.1f;						//�_���[�W��(Damage Per Second)
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
		AdditionEffect* effect = nullptr;				//�G�t�F�N�g���
	};
}

namespace EnemyHigeHorming									//�ǐՒe���
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
namespace EnemyHigeTeleportation						//�e���|�[�g���
{
	struct ENEMY_PARAMETER
	{
		CEffekseer* effect = nullptr;							//�e���|�[�g�G�t�F�N�g
		float distance = 1.0f;										//Player�Ƃ̋���
	};
}
namespace EnemyHigeSummons							//���g�������
{
	struct ENEMY_PARAMETER
	{
		int summons_max = 10;																//��������ő吔
		int summons_num = 6;																//���g�������鐔
		float trans_length = 3.0f;																//���g���ړ����鋗��
		float trans_speed = 0.1f;																//���g���ړ�����X�s�[�h
		EnemyHige_Avater* avater = {};													//���g���
		float cube_size = 1.0f;																	//�L���[�u�̑傫��
		int avater_old_num = summons_num;										//���g�̑O��܂ł̐�
		D3DXVECTOR3 move_position = D3DXVECTOR3(0,0,0);			//�ړ�����ꏊ
		int avater_alive = 0;																		//�G�������Ă��鐔
		float cube_speed = 0.1f;																//�L���[�u�̔��ł����X�s�[�h
		int attack_interval = 180;															//�U���̊Ԋu
	};
}
namespace EnemyHigeBurstShot
{
	struct ENEMY_PARAMETER
	{
		
		CEffekseer* effect = nullptr;										//�G�t�F�N�g
		float effect_size = 1.0f;												//�G�t�F�N�g�̃T�C�Y
		OBB* collision = nullptr;												//�R���W����
		float dps = 20.0f;															//damage per second
		float blur_color[3] = { 255.0f,255.0f,255.0f};				//�u���[�̃J���[
		int blur_alpha = 240;													//�u���[�e�N�X�`���̃��l
		bool isblur = true;														//�u���[���g�p����̂�
	};
}

class EnemyHige :public Enemy
{
public:
	static const float BURSTX;
	static const float BURSTY;
	static const float BURSTZ;

	enum STATE					//��������ɒʏ�U�����L�q���邱�ƁB���ɓ���U�����L�q�B
	{
		IDLE = 0,						//�ҋ@���
		RUSH,							//�ːi
		CIRCLESHOT,				//�g��U��
		HORMING,					//�z�[�~���O�e
		TELEPORTETION,		//�e���|�[�g
		BURSTSHOT,				//�o�[�X�g�V���b�g
		SUMMONSAVATER,	//���g����
		STATE_MAX,
	};
	const char* StateWord[STATE_MAX]
	{
		"IDLE",
		"RUSH",
		"CIRCLESHOT",
		"HORMING",
		"TELEPORTETION",
		"BURSTSHOT",
		"SUMMONSAVATER",
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
	void SaveRushParameter(const std::string filename);			//�ːi�p�����[�^�ۑ�
	void LoadRushParameter(const std::string filename);			//�ːi�p�����[�^�Ǎ�

	//�g��U���̃p�����[�^�擾�E�ݒ�
	EnemyHigeCircleShot::ENEMY_PARAMETER GetCircleShotParameter();
	void SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER* parameter);
	void SaveCircleShotParameter(const std::string filename);		//�g��U���p�����[�^�ۑ�
	void LoadCircleShotParameter(const std::string filename);	//�g��U���p�����[�^�ǂݍ���
	

	//�z�[�~���O�̃p�����[�^�擾�E�ݒ�
	EnemyHigeHorming::ENEMY_PARAMETER GetHormingParameter();
	void SetHormingParameter(EnemyHigeHorming::ENEMY_PARAMETER* parameter);
	void SaveHormingParameter(const std::string filename);		//�z�[�~���O�p�����[�^�̕ۑ�
	void LoadHormingParameter(const std::string filename);		//�z�[�~���O�p�����[�^�Ǎ�

	//�e���|�[�g�̃p�����[�^�擾�E�ݒ�
	EnemyHigeTeleportation::ENEMY_PARAMETER GetTeleportParameter();
	void SetTeleportParameter(EnemyHigeTeleportation::ENEMY_PARAMETER* parameter);
	void SaveTeleportParameter(const std::string filename);		//�e���|�[�g�p�����[�^�ۑ�
	void LoadTeleportParameter(const std::string filename);		//�e���|�[�g�p�����[�^�Ǎ�

	//�����̃p�����[�^�擾�E�ݒ�
	EnemyHigeSummons::ENEMY_PARAMETER GetSummonsParameter();
	void SetSummonsParameter(EnemyHigeSummons::ENEMY_PARAMETER* parameter);
	void SaveSummonsParameter(const std::string filename);		//�����p�����[�^�ۑ�
	void LoadSummonsParameter(const std::string filename);		//�����p�����[�^�Ǎ�

	//�o�[�X�g�V���b�g�̃p�����[�^�擾�E�ݒ�
	EnemyHigeBurstShot::ENEMY_PARAMETER GetBurstParameter();
	void SetBurstParameter(EnemyHigeBurstShot::ENEMY_PARAMETER* parameter);
	void SaveBurstParameter(const std::string filename);		//�o�[�X�g�p�����[�^�ۑ�
	void LoadBurstParameter(const std::string filename);		//�o�[�X�g�p�����[�^�Ǎ�

private:
	void DrawDebug();															//Debug�\��
	void InitParameter();														//�p�����[�^������
	void ReCreateCircleParameter();									//�g��U���p�����[�^�č쐬
	void ReCreateHormingParameter();								//�z�[�~���O�p�����[�^�č쐬
	void ReCreateSummonsParameter();								//���g�����p�����[�^�č쐬
	void InitCircleParameterValue();										//�g��p�����[�^������
	void InitHormingParameterValue();								//�p�����[�^�̏ڍׂ�������
	void InitTeleportParameterValue();									//�e���|�[�g�̃p�����[�^������
	void InitSummonsParameterValue();								//���g�����p�����[�^������
	void InitBurstShotParameterValue();								//�o�[�X�g�V���b�g�̏�񏉊���
	void DestParameter();														//�p�����[�^�f�X�g���N�^
private:
	EnemyHigeUI* ui_ = nullptr;											//UI
	StatePattern_Enemy* statePattern_ = nullptr;				//�X�e�[�g��ԊǗ�
	STATE state_ = IDLE;														//���
	Sphere* collision_ = nullptr;											//�R���W�������
	EnemyHigeRush::ENEMY_PARAMETER rushParameter_ = {};								//�ːi��Ԃ̃p�����[�^���
	EnemyHigeCircleShot::ENEMY_PARAMETER circleShotParameter_ = {};			//�g��U���̃p�����[�^���
	EnemyHigeHorming::ENEMY_PARAMETER hormingParameter_ = {};					//�z�[�~���O�̃p�����[�^���
	EnemyHigeTeleportation::ENEMY_PARAMETER teleportationParameter_ = {};	//�e���|�[�g�̃p�����[�^���
	EnemyHigeSummons::ENEMY_PARAMETER summonsParameter_ = {};				//���g�����̃p�����[�^���
	EnemyHigeBurstShot::ENEMY_PARAMETER burstParameter_ = {};					//�o�[�X�g�V���b�g�̃p�����[�^���
	XModel* ring_ = nullptr;
};