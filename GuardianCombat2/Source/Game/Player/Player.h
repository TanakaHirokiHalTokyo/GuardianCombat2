#pragma once

#include "../Object.h"
class SceneSkinMesh;
class Vector3;
class Camera;
class DeviceConfig;
class Texture;
class Weapon;

const float PLAYER_DEFAULT_SPEED = 0.005f;
const float PLAYER_DEFAULT_LIFE = 100.0f;

class Player :public Object
{
public:
	const float DAMAGE_ALPHA_SPEED = 2.0f;

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
	void SetCamera(Camera* camera);						//PlayerCamera��ݒ�
	Camera* GetCamera();											//PlayerCamera���擾
	float GetLife();															//Life�擾
	void DecreaseLife(float value);								//Life���Z
	Weapon* GetWeapon();

	inline void SetInvincible(bool flag) { invincible_ = flag; }		//���G��Ԑݒ�
	inline bool GetInvincible() { return invincible_; }					//���G��Ԏ擾

	inline void SetDamageAlpha(float value) { damageAlpha_ = value; }		//�_���[�W���l�ݒ�
	inline float GetDamageAlpha() { return damageAlpha_; }							//�_���[�W���l�擾

	//�f�o�C�X���擾
	DeviceConfig* GetDeviceConfig();
protected:
	Weapon* shotgun_ = nullptr;									//�V���b�g�K�����
	float damageAlpha_ = 0.0f;										//�_���[�W���l
	DeviceConfig* deviceConfig_ = nullptr;					//�f�o�C�X���
	bool invincible_ = false;											//���G
	Camera* camera_ = nullptr;									//�J�������
	SceneSkinMesh* animModel_ = nullptr;				//�A�j���[�V�����t�����f��
	float speed_ = PLAYER_DEFAULT_SPEED;			//�v���C���[�X�s�[�h
	float life_ = PLAYER_DEFAULT_LIFE;					//�v���C���[�̗�
	Vector3* vector_ = nullptr;										//�v���C���[�x�N�g��
	bool onLand_ = true;												//�n�ʂɏ���Ă��邩
};