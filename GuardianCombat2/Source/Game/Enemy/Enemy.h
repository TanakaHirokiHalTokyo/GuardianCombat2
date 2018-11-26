#pragma once

#include "../Object.h"
#include "../../main.h"
#include <array>

class Vector3;
class XModel;

//�ҋ@���
namespace EnemyIdle
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.01f;						//�X�s�[�h
		float rot_angle = 0.1f;					//��]�p�x
		std::array<int, 5> count = { 120, 100,80,60,40 };
		std::array<float, 4> hp_ratio_ = { 0.8f,0.6f,0.4f,0.2f };												//HP�̊���
		std::array<double, 5> normalAttackLuck = { 0.9,0.9,0.9,0.9,0.9 };
		std::array<double, 5> specialAttackLuck = { 0.1 ,0.1,0.1,0.1,0.1};
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

	void SetDebugMode(bool flag) { debug_ = flag; }					//�f�o�b�O���[�h�ݒ�
	bool GetDebugMode() { return debug_; }						//�f�o�b�O���[�h�擾

	EnemyIdle::ENEMY_PARAMETER GetIdleParameter();
	void SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter);

	inline float GetMoveSpeedToPoint() { return moveSpeedToPoint_; }
	inline ENEMY_TYPE GetEnemyType() { return enemyType_; }
	inline size_t GetNormalAttackNum() { return normalAttackNum_; }
	inline size_t GetSpecialAttackNum() { return specialAttackNum_; }
	inline bool GetAutoAttack() { return autoAttack_; }
protected:
	ENEMY_TYPE enemyType_ = ENEMY_MAX;																//�G�̎��
	float life_ = ENEMY_MAX_LIFE;																										//�̗�
	Vector3* vector_ = nullptr;																							//�x�N�g�����
	XModel* model_ = nullptr;																							//���f�����
	bool debug_ = false;																									//�f�o�b�O��Ԃɂ��邩
	bool enemypause_ = false;																							//�G�̃|�[�Y���
	EnemyIdle::ENEMY_PARAMETER idleParameter_ = {};												//�ҋ@��Ԃ̃p�����[�^
	float moveSpeedToPoint_ = 0.1f;																				//�ړ��n�_�Ɉړ�����X�s�[�h
	size_t normalAttackNum_ = 0;																				//�ʏ�U���̐�
	size_t specialAttackNum_ = 0;																				//����U���̐�
	bool autoAttack_ = false;																						//�����I�ɍU�������Ă��邩
};