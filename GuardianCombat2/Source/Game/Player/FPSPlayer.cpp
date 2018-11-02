#include "FPSPlayer.h"
#include "../../Camera/FPSCamera.h"
#include "../../Vector3/Vector3.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"
#include "../../Imgui/ImguiManager.h"
#include "StatePlayer\FPSPlayer_Movement.h"
#include "Weapon\Weapon_Shotgun.h"
#include "UI\FPSPlayer_UI.h"

FPSPlayer::FPSPlayer()
{
	//�x�N�g���i�[��쐬
	vector_ = new Vector3();

	//�ړ�����쐬
	movement_ = new FPSPlayerMovement();

	//�J�����쐬
	camera_ = Object::Create<FPSCamera>();
	FPSCamera*  FpsCamera = (FPSCamera*)camera_;
	FpsCamera->SetPlayer(this);

	//����쐬
	shotgun_ = Object::Create<Weapon_Shotgun>();
	shotgun_->SetPlayer(this);

	//UI�쐬
	playerUI_ = new FPSPlayer_UI();
	playerUI_->Init();
}

FPSPlayer::~FPSPlayer()
{
	//�x�N�g���j��
	delete vector_;

	//�ړ�����j��
	delete movement_;

	//UI�j��
	delete playerUI_;
}

void FPSPlayer::Init()
{
	SetPosition(0.0f,0.0f, 8.0f);
	SetRotation(0.0f,0.0f,0.0f);
	SetScale(1.0f,1.0f,1.0f);

	//�v���C���[UI������
	playerUI_->Init();
}

void FPSPlayer::Uninit()
{
	//�v���C���[UI�I������
	playerUI_->Uninit();
}

void FPSPlayer::Update()
{
	//�ړ������X�V
	movement_->Act(this);

	//UI�X�V
	playerUI_->Update();
}

void FPSPlayer::BeginDraw()
{
	D3DXMATRIX mtxRotate, mtxTrans, mtxScale;
	D3DXMatrixTranslation(&mtxTrans,GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, GetRotate().x, GetRotate().y, GetRotate().z);
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);

	world_ = mtxScale;
	world_ *= mtxRotate;
	world_ *= mtxTrans;
}

void FPSPlayer::Draw()
{
	//�v���C���[UI�`��
	playerUI_->Draw();
}

void FPSPlayer::EndDraw()
{

}

Weapon * FPSPlayer::GetWeapon()
{
	return shotgun_;
}
