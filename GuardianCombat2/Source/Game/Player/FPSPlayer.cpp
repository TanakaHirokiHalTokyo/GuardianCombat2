#include "FPSPlayer.h"
#include "../../Camera/FPSCamera.h"
#include "../../Vector3/Vector3.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"
#include "../../Imgui/ImguiManager.h"
#include "StatePlayer\FPSPlayer_Movement.h"

FPSPlayer::FPSPlayer()
{
	//�x�N�g���i�[��쐬
	vector_ = new Vector3();

	//�ړ�����쐬
	movement_ = new FPSPlayerMovement();

	//�J�����쐬
	camera_ = new FPSCamera();
	FPSCamera*  FpsCamera = (FPSCamera*)camera_;
	FpsCamera->SetPlayer(this);
}

FPSPlayer::~FPSPlayer()
{
	//�x�N�g���j��
	delete vector_;

	//�ړ�����j��
	delete movement_;

	//�J�����j��
	camera_->Uninit();
	delete camera_;
	camera_ = nullptr;
}

void FPSPlayer::Init()
{
	SetPosition(0.0f,0.0f, 8.0f);
	SetRotation(0.0f,0.0f,0.0f);
	SetScale(1.0f,1.0f,1.0f);

	//�J����������
	camera_->Init();
}

void FPSPlayer::Uninit()
{
	//�J�����I������
	camera_->Uninit();
}

void FPSPlayer::Update()
{
	//�ړ������X�V
	movement_->Act(this);

	//�J�����X�V
	camera_->Update();
}

void FPSPlayer::BeginDraw()
{
	camera_->BeginDraw();
}

void FPSPlayer::Draw()
{
	//�J�����`�揈��
	camera_->Draw();
}

void FPSPlayer::EndDraw()
{
	//�J�����`��I��������
	camera_->EndDraw();
}

Camera * FPSPlayer::GetCamera()
{
	return camera_;
}	
