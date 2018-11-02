#pragma once

#include "../Object.h"

class Vector3;
class XModel;

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

	void SetLife(float value) { life_ = value; }	//�̗͐ݒ�
	float GetLife() { return life_; }				//�̗͎擾
	void SetSpeed(float value) { speed_ = value; }	//�X�s�[�h�ݒ�
	float GetSpeed() { return speed_; }				//�X�s�[�h�擾

	XModel* GetModel();								//���f���擾
	Vector3* GetVector();							//�x�N�g���擾

	void SetDebugMode(bool flag) { debug_ = flag; }
	bool GetDebugMode() { return debug_; }
protected:
	float life_ = 100.0f;							//�̗�
	float speed_ = 0.01f;							//�X�s�[�h
	Vector3* vector_ = nullptr;						//�x�N�g�����
	XModel* model_ = nullptr;						//���f�����
	bool debug_ = false;							//�f�o�b�O��Ԃɂ��邩
};