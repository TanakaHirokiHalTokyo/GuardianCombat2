#pragma once

#include "Player.h"


class Camera;
class FPSPlayerState;

class FPSPlayer :public Player
{
public:
	const float FPSPLAYER_DEF_SPEED = 0.05f;		//Player�̃f�t�H���g�X�s�[�h

	FPSPlayer();					//�R���X�g���N�^
	~FPSPlayer()override;			//�f�X�g���N�^
	void Init()override;			//����������
	void Uninit()override;			//�I������
	void Update()override;			//�X�V����
	void BeginDraw()override;		//���O�`�揈��
	void Draw()override;			//�`�揈��
	void EndDraw()override;			//�I�u�W�F�N�g���ׂĂ̕`�悪�I�������̏���

	Camera* GetCamera();

private:
	Camera* camera_ = nullptr;				//�J�������
	FPSPlayerState* movement_ = nullptr;	//�v���C���[�̈ړ��𐧌�
};