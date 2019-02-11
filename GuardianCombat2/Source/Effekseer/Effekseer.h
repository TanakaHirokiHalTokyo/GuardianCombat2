#pragma once

#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <d3d9.h>
#include <d3dx9.h>

struct EffectFile
{
	const wchar_t fileName[256];
};


class CEffekseer
{
public:
	struct Transform
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scale;
		D3DXVECTOR3 rotate;
	};

	const enum Effect
	{
		Effect_Bullet,							//�e�̃G�t�F�N�g
		Effect_Teleport,						//�e���|�[�g
		Effect_Burst,							//�o�[�X�g
		Effect_EnemyExplosion,			//�G�̔����G�t�F�N�g
		Effect_HitEnemy,						//�G�Ƀq�b�g�����Ƃ�
		Effect_Max
	};
	//�t�@�C���p�X
	const EffectFile fileName_[Effect_Max]{
		L"resource/Effekseer/simple_bullet.efk",
		L"resource/Effekseer/teleportation.efk",
		L"resource/Effekseer/burstshot.efk",
		L"resource/Effekseer/EnemyExplosion.efk",
		L"resource/Effekseer/Hit.efk",
	};


public:
	CEffekseer(Effect EffectType);					//�R���X�g���N�^�@�G�t�F�N�g���w��
	~CEffekseer();												//�f�X�g���N�^
	void Init();													//����������
	void Uninit();												//�I������
	void Update();												//�X�V����
	void Draw();													//�`�揈��
	
	void SetSpeed(float speed);						//�Đ��X�s�[�h�ݒ�
	void RepeatEffect(bool repeat);					//���s�[�g���邩�ݒ�
	void Play();													//�Đ�
	void Paused(bool paused);							//�|�[�Y�ݒ�
	void Stop();													//��~
	bool GetPlaying();										//�Đ����Ă��邩
	void SetVisible(bool flag);							//����Ԑݒ�
	bool GetVisible();										//����Ԏ擾
	void SetPosition(float x,float y,float z);		//���W�ݒ�
	void SetPosition(D3DXVECTOR3 trans);	//���W�ݒ�
	D3DXVECTOR3 GetPosition();					//���W�擾
	void SetRotate(float x,float y,float z);			//��]�ݒ�
	void SetRotate(D3DXVECTOR3 rot);			//��]�ݒ�
	D3DXVECTOR3 GetRotate();						//��]�擾
	void SetScale(float x, float y,float z);			//�X�P�[���l�ݒ�
	void SetScale(D3DXVECTOR3 scale);		//�X�P�[���l�ݒ�
	D3DXVECTOR3 GetScale();						//�X�P�[���l�擾

private:
	void LoadEffect();										//�G�t�F�N�g�ǂݍ���
	void SetView(D3DXVECTOR3 Pos, D3DXVECTOR3 At, D3DXVECTOR3 Up);								//�r���[�s��ݒ�
	void SetProj(float Fov, float ScreenWidth, float ScreenHeight, float Near, float Far);					//�v���W�F�N�V�����Z�b�g
	void SetMatrix(D3DXMATRIX mat);																								//Effekseer�̃��[���h�ϊ��ݒ�

	Effekseer::Manager*					effekseerManager_;						//�G�t�F�N�g�}�l�[�W���[
	EffekseerRendererDX9::Renderer*		effekseerRenderer_;				//�G�t�F�N�g�`��f�o�C�X
	Effekseer::Effect*					effekseerEffect_;									//�G�t�F�N�g�f�o�C�X
	Effekseer::Handle					effekseerHandle_;								//�n���h���[
	CEffekseer::Effect					effectType_;
	bool repeat_ = false;				//�J�肩�����Đ����邩�B
	bool paused_ = false;				//�|�[�Y�����B
	bool playing_ = false;				//�Đ�����
	bool visible_ = false;				//�\�����邩
	float speed_ = 1.0f;
	Transform transform_;

};