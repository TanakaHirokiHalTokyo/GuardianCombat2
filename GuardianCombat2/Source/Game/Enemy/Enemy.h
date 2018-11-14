#pragma once

#include "../Object.h"

class Vector3;
class XModel;

//�ҋ@���
namespace EnemyIdle
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.01f;						//�X�s�[�h
		float rot_angle = 0.1f;						//��]�p�x
	};
}

class Enemy :public Object
{
public:
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

	void SetDebugMode(bool flag) { debug_ = flag; }					//�f�o�b�O���[�h�ݒ�
	bool GetDebugMode() { return debug_; }						//�f�o�b�O���[�h�擾

	EnemyIdle::ENEMY_PARAMETER GetIdleParameter();
	void SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter);

protected:
	float life_ = 100.0f;										//�̗�
	Vector3* vector_ = nullptr;								//�x�N�g�����
	XModel* model_ = nullptr;								//���f�����
	bool debug_ = false;										//�f�o�b�O��Ԃɂ��邩
	bool enemypause_ = false;								//�G�̃|�[�Y���
	EnemyIdle::ENEMY_PARAMETER idleParameter_ = {};	//�ҋ@��Ԃ̃p�����[�^

};