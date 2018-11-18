#include "FPSPlayer.h"
#include "../../Camera/FPSCamera.h"
#include "../../Vector3/Vector3.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"
#include "../../Imgui/ImguiManager.h"
#include "StatePlayer\FPSPlayer_Movement.h"
#include "Weapon\Weapon_Shotgun.h"
#include "UI\FPSPlayer_UI.h"
#include "../../Collision/Collision.h"
#include "PlayerLifeBar.h"
#include "../MeshField/MeshField.h"

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

	//�R���W�����쐬
	collision_ = AddCollision();
	collision_->object_ = this;

	//���C�t�o�[�쐬
	lifeBar_ = Object::Create<PlayerLifeBar>(this);
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
	SetPosition(0.1f,0.0f, 8.0f);
	SetRotation(0.0f,0.0f,0.0f);
	SetScale(1.0f, 1.0f, 1.0f);

	collision_->pos = GetPosition();
	collision_->rad = 0.01f;

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

	//�͈͓��Ɏ��߂�
	float lengthX = fabsf(GetPosition().x);
	float lengthZ = fabsf(GetPosition().z);

	if (lengthX > FIELD_SIZE)
	{
		if (GetPosition().x > 0) { SetPositionX(FIELD_SIZE); }
		else { SetPositionX(-FIELD_SIZE); }
	}
	if (lengthZ > FIELD_SIZE)
	{
		if (GetPosition().z > 0) { SetPositionZ(FIELD_SIZE); }
		else { SetPositionZ(-FIELD_SIZE); }
	}

	//CollisionUpdate
	collision_->pos = GetPosition();
	collision_->pos.y = GetPosition().y + collision_->rad;
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

	ImGui::Begin("Player Position");
	ImGui::Text("Position %f %f %f", GetPosition().x, GetPosition().y, GetPosition().z);
	ImGui::End();
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

Sphere* FPSPlayer::AddCollision()
{
	Sphere* collision = new Sphere();
	playerCollision_.emplace_back(collision);
	return collision;
}
