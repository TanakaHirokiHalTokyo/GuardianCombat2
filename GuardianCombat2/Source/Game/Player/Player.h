#pragma once

#include "../Object.h"
class SceneSkinMesh;
class Vector3;

constexpr float PLAYER_DEFAULT_SPEED = 0.005f;
constexpr float PLAYER_DEFAULT_LIFE = 100.0f;

class Player :public Object
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	SceneSkinMesh* GetAnimModel() {
		if (animModel_ != nullptr)
		{
			return animModel_;
		}
		return nullptr;
	}
	Vector3* GetVector();

	void SetOnLand(bool flag);
	bool GetOnLand();
protected:
	SceneSkinMesh* animModel_ = nullptr;			//�A�j���[�V�����t�����f��
	float speed_ = PLAYER_DEFAULT_SPEED;			//�v���C���[�X�s�[�h
	float life_ = PLAYER_DEFAULT_LIFE;				//�v���C���[�̗�
	Vector3* vector_ = nullptr;						//�v���C���[�x�N�g��
	bool onLand_ = true;							//�n�ʂɏ���Ă��邩
};