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

	//各情報初期化
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	vector_->SetFront(0.0f, 0.0f, -1.0f);
	vector_->SetRight(-1.0f, 0.0f, 0.0f);
	vector_->SetUp(0.0f, 1.0f, 0.0f);

	//プレイヤー情報をもとにカメラ情報セット
	if (player_ != nullptr)
	{
		//PlayerPosition Get
		const auto& player_position = player_->GetPosition();
		D3DXVECTOR3 camera_position = player_position;
		camera_position.y += INIT_HEIGHT;

		SetPosition(camera_position);
	}
	//プロジェクションセット
	SetProjection();
}

void FPSCamera::Uninit()
{
}

void FPSCamera::Update()
{
	//プレイヤー情報セット
	if (player_ != nullptr)
	{
		//PlayerPosition Get
		const auto& player_position = player_->GetPosition();
		D3DXVECTOR3 camera_position = player_position;
		camera_position.y += INIT_HEIGHT;

		SetPosition(camera_position);

		at_ = camera_position + vector_->GetFront() * 10.0f;
	}

	//カメラ中視点更新
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

	//マウスによるカメラ制御
	MouseRotate.x = (float)MouseData.lX * mouseSensi_;	//X方向への移動量 * 感度
	MouseRotate.y = (float)MouseData.lY * mouseSensi_;	//Y方向への移動量 * 感度
	MouseRotate.z = (float)MouseData.lZ * mouseSensi_;	//ホイール移動量　* 感度

	//コントローラー　管理
	float controllerLX = (float)X_CONTROLLER::GetXcontrollerLStickX(1);
	float controllerLY = (float)X_CONTROLLER::GetXcontrollerLStickY(1);
	float controllerRX = (float)X_CONTROLLER::GetXcontrollerRStickX(1);
	float controllerRY = (float)X_CONTROLLER::GetXcontrollerRStickY(1);

	D3DXMATRIX mtxRotX;
	D3DXMATRIX mtxRotY;
	D3DXVECTOR3 RotVec;

	//回転行列を作成
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

	//向きベクトル
	RotVec = at_ - transform_.pos;
	
	//ベクトル回転
	D3DXVec3TransformNormal(&RotVec,&RotVec,&mtxRotY);


	D3DXVECTOR3 OldCamAt;
	OldCamAt = at_;

	at_ = transform_.pos + RotVec;

	vector_->SetFront(at_ - transform_.pos);

	D3DXVECTOR3 dir = vector_->GetFront();
	dir.y = 0;

	D3DXVec3Normalize(&dir, &dir);

	//内積を求める
	float DotAngle = D3DXVec3Dot(&vector_->GetFront(), &dir);
	if (DotAngle <= 0.5f || DotAngle > 1.0f)
	{
		at_ = OldCamAt;
	}


	dir = vector_->GetFront();
	dir.y = 0;

	D3DXVec3Normalize(&dir, &dir);


	vector_->SetFront(at_ - transform_.pos);

	//右ベクトル
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

	//向きベクトル
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

	//前ベクトル
	vector_->SetFront(at_ - transform_.pos);

	//右ベクトル
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);

}
