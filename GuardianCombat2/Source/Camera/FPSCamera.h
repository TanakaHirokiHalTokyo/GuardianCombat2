#pragma once

#include "Camera.h"
#include "../Game/Player/Player.h"

class DeviceConfig;

class FPSCamera :public Camera
{
public:
	FPSCamera();							//�R���X�g���N�^
	~FPSCamera();							//�f�X�g���N�^
	void Init()override;					//������
	void Uninit()override;					//�I������
	void Update()override;					//�X�V
	void BeginDraw()override;				//���O�`��
	void Draw()override;					//�`��
	void EndDraw()override;					//���ׂẴI�u�W�F�N�g���`�悵�I������ۂ̏���

	void SetPlayer(Player* player);

private:
	void CameraAtUpdate();							//�����_�X�V

	const float INIT_HEIGHT = 0.5f;					//���������W
	const float CAMERA_AT_LENGTH = 100.0f;			//�����_�̋���
	Player* player_ = nullptr;						//Player���
	D3DXVECTOR3 MouseRotate = D3DXVECTOR3(0,0,0);	//�}�E�X�����]�p
	DeviceConfig* deviceConfig_;					//�f�o�C�X���
	float sensitivity_ = 2.0f;						//�R���g���[���[���x
	float mouseSensi_ = 0.1f;						//�}�E�X���x
};