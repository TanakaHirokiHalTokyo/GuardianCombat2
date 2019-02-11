#pragma once

#include "Player.h"


class Camera;
class FPSPlayerState;
class Weapon;
class FPSPlayer_UI;
class Sphere;

class FPSPlayer :public Player
{
public:
	const float FPSPLAYER_DEF_SPEED = 0.05f;		//Player�̃f�t�H���g�X�s�[�h

	FPSPlayer();							//�R���X�g���N�^
	~FPSPlayer()override;			//�f�X�g���N�^
	void Init()override;					//����������
	void Uninit()override;				//�I������
	void Update()override;			//�X�V����
	void BeginDraw()override;		//���O�`�揈��
	void Draw()override;				//�`�揈��
	void EndDraw()override;			//�I�u�W�F�N�g���ׂĂ̕`�悪�I�������̏���

	Weapon* GetWeapon();

	Sphere* AddCollision();

private:
	bool debug_ = false;								//�f�o�b�O���[�h
	FPSPlayerState* movement_ = nullptr;	//�v���C���[�̈ړ��𐧌�
	Weapon* shotgun_ = nullptr;					//�V���b�g�K�����
	FPSPlayer_UI* playerUI_ = nullptr;		//PlayerUI���
	Sphere* collision_ = nullptr;					//PlayerCollision
};