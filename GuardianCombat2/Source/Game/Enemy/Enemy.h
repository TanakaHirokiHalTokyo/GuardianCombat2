#pragma once

#include "../Object.h"
#include "../../main.h"
#include <array>
#include <string>

class Vector3;
class XModel;
class Sound;
class CEffekseer;

//�ҋ@���
namespace EnemyIdle
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.01f;						//�X�s�[�h
		float rot_angle = 0.1f;					//��]�p�x
		float approache_length = 1.0f;	//�v���C���[�Ƃ̋����i����ȏ�͋߂Â��Ȃ��j
		std::array<int, 5> count = { 120, 100,80,60,40 };
		std::array<float, 4> hp_ratio_ = { 0.8f,0.6f,0.4f,0.2f };												//HP�̊���
		std::array<double, 5> normalAttackLuck = { 0.9,0.8,0.7,0.65,0.6 };
		std::array<double, 5> specialAttackLuck = { 0.1 ,0.2,0.3,0.35,0.4};
		int idle__counter = 0;
	};
}

const float ENEMY_MAX_LIFE = 100.0f;

class Enemy :public Object
{
public:

	enum ENEMY_TYPE
	{
		ENEMY_HIGE,
		ENEMY_MAX
	};

	virtual ~Enemy();
	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	void SetLife(float value) { life_ = value; }						//�̗͐ݒ�
	void DecreaseLife(float value) {
		life_ -= value;
		if (life_ < 0.0f) life_ = 0.0f;
	}
	float GetLife() { return life_; }									//�̗͎擾
	void SetSpeed(float value) { idleParameter_.speed = value; }		//�X�s�[�h�ݒ�
	float GetSpeed() { return idleParameter_.speed; }				//�X�s�[�h�擾
	void JudgePause();

	XModel* GetModel();											//���f���擾
	Vector3* GetVector();											//�x�N�g���擾
	void SetVector(Vector3 vector);							//�x�N�g���ݒ�
	void SetFront(D3DXVECTOR3 front);
	void SetUp(D3DXVECTOR3 up);
	void SetRight(D3DXVECTOR3 right);

	void SetPositionExploose(D3DXVECTOR3 pos);		//�����G�t�F�N�g���W�ݒ�
	void SetPositionExploose(float x,float y,float z);
	void PlayExploose();				//�������Đ�
	void StopExploose();				//��������~

	EnemyIdle::ENEMY_PARAMETER GetIdleParameter();									//�ҋ@��ԃp�����[�^�擾
	void SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter);		//�ҋ@��ԃp�����[�^�ݒ�

	inline float GetMoveSpeedToPoint() { return moveSpeedToPoint_; }				//�ݒ肵���ꏊ�Ɍ������X�s�[�h
	inline ENEMY_TYPE GetEnemyType() { return enemyType_; }						//�G�̎�ގ擾
	inline size_t GetNormalAttackNum() { return normalAttackNum_; }				//�ʏ�U���̐��擾
	inline size_t GetSpecialAttackNum() { return specialAttackNum_; }				//����U���̐��擾
	inline void SetAutoAttack(bool flag) { autoAttack_ = flag; }							//�I�[�g�A�^�b�N�ݒ�
	inline bool GetAutoAttack() { return autoAttack_; }										//�I�[�g�A�^�b�N���Ă���̂��擾
	inline void SetInvincible(bool flag) { invincible_ = flag; }								//���G��Ԑݒ�
	inline bool GetInvincible() { return invincible_; }											//���G��Ԃ��擾
	inline void SetEditMode(bool flag) { isEdit_ = flag; }										//EditMode�ݒ�
	inline bool GetEditMode()const { return isEdit_; }											//EditMode�擾
	inline void SetCollisionEnable(bool flag) { isCollisionEnable_ = flag; }			//Collision�̓����蔻��s�����ݒ�
	inline bool GetCollisionEnable() { return isCollisionEnable_; }						//Collision�̓����蔻��s�����擾
	inline void SetCollisionDPS(float value) { collisionDPS_ = value; }				//Collision�̓����蔻��̍ۂ�DPS�ݒ�
	inline float GetCollisionDPS() { return collisionDPS_; }									//Collision�̓����蔻��̍ۂ�DPS�擾
	inline void SetDamageCutRate(float rate) { damageCutRate_ = rate; }			//�_���[�W�J�b�g���ݒ�
	inline float GetDamageCutRate() { return damageCutRate_; }						//�_���[�W�J�b�g���擾
protected:
	void SaveIdleParameter(std::string filename);												//�ҋ@��ԃp�����[�^�ۑ�
	void LoadIdleParameter(std::string filename);												//�ҋ@��ԃp�����[�^�ǂݍ���
	void SaveBasisParameter(std::string filename);												//��{���p�����[�^�ۑ�
	void LoadBasisParameter(std::string filename);											//��{���p�����[�^�ǂݍ���

	CEffekseer* exploose_ = nullptr;																				//�����G�t�F�N�g
	Sound* explooseSE_ = nullptr;																					//������	�i�q�b�g���j
	float maxLife_ = ENEMY_MAX_LIFE;																			//�ő�̗�
	float damageCutRate_ = 0.0f;																						//�_���[�W�J�b�g��
	bool isEdit_ = false;																									//EditMode��
	bool isCollisionEnable_ = false;																					//Player�Ƒ̂��Ԃ������Ƃ��A������s����
	float collisionDPS_ = 0.0f;																							//�R���W����������s���ۂ�DPS
	ENEMY_TYPE enemyType_ = ENEMY_MAX;																//�G�̎��
	float life_ = ENEMY_MAX_LIFE;																					//�̗�
	Vector3* vector_ = nullptr;																							//�x�N�g�����
	XModel* model_ = nullptr;																							//���f�����
	bool enemypause_ = false;																							//�G�̃|�[�Y���
	EnemyIdle::ENEMY_PARAMETER idleParameter_;												//�ҋ@��Ԃ̃p�����[�^
	float moveSpeedToPoint_ = 0.1f;																				//�ړ��n�_�Ɉړ�����X�s�[�h
	size_t normalAttackNum_ = 0;																				//�ʏ�U���̐�
	size_t specialAttackNum_ = 0;																				//����U���̐�
	bool autoAttack_ = false;																						//�����I�ɍU�������Ă��邩
	bool invincible_ = false;																							//���G�t���O
};