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
#include "../MeshField/MeshField.h"
#include "UI\FPSPlayer_UI.h"
#include "../../main.h"
#include "DeviceConfig\DeviceConfig.h"
#include "../../Texture/Texture.h"

FPSPlayer::FPSPlayer()
{
	//カメラ作成
	camera_ = Object::Create<FPSCamera>();
	FPSCamera*  FpsCamera = (FPSCamera*)camera_;
	FpsCamera->SetPlayer(this);

	//ベクトル格納先作成
	vector_ = new Vector3();
	vector_->SetFront(camera_->GetFront());
	vector_->SetRight(camera_->GetRight());
	vector_->SetUp(camera_->GetUp());

	//移動制御作成
	movement_ = new FPSPlayerMovement();

	//武器作成
	shotgun_ = new Weapon_Shotgun();
	shotgun_->Init();
	shotgun_->SetPlayer(this);

	//UI作成
	playerUI_ = new FPSPlayer_UI();
	playerUI_->Init();

	//コリジョン作成
	collision_ = AddCollision();
	collision_->object_ = this;

	//デバイス情報作成
	deviceConfig_ = new DeviceConfig();
}

FPSPlayer::~FPSPlayer()
{
	//ベクトル破棄
	SAFE_DELETE(vector_);

	//移動制御破棄
	SAFE_DELETE(movement_);

	//UI破棄
	SAFE_DELETE(playerUI_);

	//Device情報破棄
	SAFE_DELETE(deviceConfig_);

	if (shotgun_)
	{
		shotgun_->Uninit();
		delete shotgun_;
		shotgun_ = nullptr;
	}
}

void FPSPlayer::Init()
{
	life_ = 100.0f;

	visible_ = true;

	SetPosition(0.1f,0.0f, 8.0f);
	SetRotation(0.0f,0.0f,0.0f);
	SetScale(1.0f, 1.0f, 1.0f);

	collision_->pos = GetPosition();
	collision_->rad = 0.1f;
	collision_->pos.y += collision_->rad / 2.0f;

	//プレイヤーUI初期化
	playerUI_->Init();

	shotgun_->Init();

	debug_ = false;

	if (GameManager::GetSceneTag() == "EditScene")
	{
		debug_ = true;
	}
}

void FPSPlayer::Uninit()
{
	//プレイヤーUI終了処理
	playerUI_->Uninit();

	shotgun_->Uninit();
}

void FPSPlayer::Update()
{
	if (life_ <= 0.0f)
	{
		GameManager::GameOver(true);
		return;
	}

	shotgun_->Update();

	//移動処理更新
	movement_->Act(this);

	//UI更新
	playerUI_->Update();

	//範囲内に収める
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

	if(visible_)	shotgun_->BeginDraw();
}

void FPSPlayer::Draw()
{
	if(visible_)	shotgun_->Draw();

	if (GameManager::GetEnableEdit())
	{
		if (debug_)
		{
			ImGui::Begin(u8"プレイヤー情報");
			ImGui::Checkbox(u8"無敵", &invincible_);
			ImGui::SliderFloat(u8"プレイヤー体力", &life_, 0, 100.0f);
			ImGui::Text(u8"座標 %f %f %f", GetPosition().x, GetPosition().y, GetPosition().z);
			ImGui::End();
		}
	}
}

void FPSPlayer::EndDraw()
{
	shotgun_->EndDraw();
	//プレイヤーUI描画
	playerUI_->Draw();
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
