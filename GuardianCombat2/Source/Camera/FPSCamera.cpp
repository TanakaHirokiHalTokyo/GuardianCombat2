#include "FPSCamera.h"
#include "../Game/Player/Player.h"
#include "../Vector3/Vector3.h"
#include "../DInput/DirectInput.h"
#include "../XInput/xcontroller.h"
#include "../Game/Player/DeviceConfig/DeviceConfig.h"
#include "../Game/MeshField/MeshField.h"
#include "../Imgui/ImguiManager.h"

FPSCamera::FPSCamera()
{
	deviceConfig_ = new DeviceConfig();
}

FPSCamera::~FPSCamera()
{
	delete deviceConfig_;
}

void FPSCamera::Init()
{
	this->at_ = D3DXVECTOR3(0.0f, INIT_HEIGHT, -10.0f);

	//�e��񏉊���
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	vector_->SetFront(0.0f, 0.0f, -1.0f);
	vector_->SetRight(-1.0f, 0.0f, 0.0f);
	vector_->SetUp(0.0f, 1.0f, 0.0f);

	//�v���C���[�������ƂɃJ�������Z�b�g
	if (player_ != nullptr)
	{
		//PlayerPosition Get
		const auto& player_position = player_->GetPosition();
		D3DXVECTOR3 camera_position = player_position;
		camera_position.y += INIT_HEIGHT;

		SetPosition(camera_position);
	}
	//�v���W�F�N�V�����Z�b�g
	SetProjection();
}

void FPSCamera::Uninit()
{
}

void FPSCamera::Update()
{
	//�v���C���[���Z�b�g
	if (player_ != nullptr)
	{
		//PlayerPosition Get
		const auto& player_position = player_->GetPosition();
		D3DXVECTOR3 camera_position = player_position;
		camera_position.y += INIT_HEIGHT;

		SetPosition(camera_position);

		at_ = camera_position + vector_->GetFront() * 10.0f;
	}

	//�J���������_�X�V
	CameraAtUpdate();

	this->SetMainCameraInfo(this->transform_.pos, this->at_, vector_->GetUp(), this->mtxView_, this->mtxProj_);
	SetView();
}

void FPSCamera::BeginDraw()
{
}

void FPSCamera::Draw()
{
	
}

void FPSCamera::EndDraw()
{
}

void FPSCamera::SetPlayer(Player * player)
{
	player_ = player;
}

void FPSCamera::CameraAtUpdate()
{
	DIMOUSESTATE MouseData = ReturnMouseMove();

	mouseSensi_ = deviceConfig_->GetMouseSensitivity();
	sensitivity_ = deviceConfig_->GetControllerSensitivity();

	float FieldHeight = MeshField::GetHeight(transform_.pos);

	//�}�E�X�ɂ��J��������
	MouseRotate.x = (float)MouseData.lX * mouseSensi_;	//X�����ւ̈ړ��� * ���x
	MouseRotate.y = (float)MouseData.lY * mouseSensi_;	//Y�����ւ̈ړ��� * ���x
	MouseRotate.z = (float)MouseData.lZ * mouseSensi_;	//�z�C�[���ړ��ʁ@* ���x

	//�R���g���[���[�@�Ǘ�
	float controllerLX = (float)X_CONTROLLER::GetXcontrollerLStickX(1);
	float controllerLY = (float)X_CONTROLLER::GetXcontrollerLStickY(1);
	float controllerRX = (float)X_CONTROLLER::GetXcontrollerRStickX(1);
	float controllerRY = (float)X_CONTROLLER::GetXcontrollerRStickY(1);

	D3DXMATRIX mtxRotX;
	D3DXMATRIX mtxRotY;
	D3DXVECTOR3 RotVec;

	//��]�s����쐬
	if (X_CONTROLLER::GetConnectController())
	{
		transform_.rotate.y -= -controllerRY * sensitivity_;
		D3DXMatrixRotationAxis(&mtxRotY, &vector_->GetRight(), D3DXToRadian(-controllerRY * sensitivity_));
	}
	else
	{
		transform_.rotate.y -= MouseRotate.y;
		D3DXMatrixRotationAxis(&mtxRotY, &vector_->GetRight(), D3DXToRadian(MouseRotate.y));
	}

	//�����x�N�g��
	RotVec = at_ - transform_.pos;
	
	//�x�N�g����]
	D3DXVec3TransformNormal(&RotVec,&RotVec,&mtxRotY);


	D3DXVECTOR3 OldCamAt;
	OldCamAt = at_;

	at_ = transform_.pos + RotVec;

	vector_->SetFront(at_ - transform_.pos);

	D3DXVECTOR3 dir = vector_->GetFront();
	dir.y = 0;

	D3DXVec3Normalize(&dir, &dir);

	//���ς����߂�
	float DotAngle = D3DXVec3Dot(&vector_->GetFront(), &dir);
	if (DotAngle <= 0.5f || DotAngle > 1.0f)
	{
		at_ = OldCamAt;
	}


	dir = vector_->GetFront();
	dir.y = 0;

	D3DXVec3Normalize(&dir, &dir);


	vector_->SetFront(at_ - transform_.pos);

	//�E�x�N�g��
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);

	if (X_CONTROLLER::GetConnectController())
	{
		transform_.rotate.x += -controllerRX * sensitivity_;
		D3DXMatrixRotationY(&mtxRotX, D3DXToRadian(controllerRX * sensitivity_));
	}
	else
	{
		transform_.rotate.x += MouseRotate.x;
		D3DXMatrixRotationY(&mtxRotX, D3DXToRadian(MouseRotate.x));
	}

	//�����x�N�g��
	RotVec = at_ - transform_.pos;

	D3DXVec3TransformNormal(&RotVec,&RotVec,&mtxRotX);

	OldCamAt = at_;

	at_ = transform_.pos + RotVec;

	vector_->SetFront(at_ - transform_.pos);

	dir.y = 0;

	D3DXVec3Normalize(&dir, &dir);

	DotAngle = D3DXVec3Dot(&vector_->GetFront(), &dir);

	if (DotAngle <= 0.5f || DotAngle > 1.0f)
	{
		at_ = OldCamAt;
	}

	dir = vector_->GetFront();
	dir.y = 0;

	D3DXVec3Normalize(&dir, &dir);

	//�O�x�N�g��
	vector_->SetFront(at_ - transform_.pos);

	//�E�x�N�g��
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);

}
