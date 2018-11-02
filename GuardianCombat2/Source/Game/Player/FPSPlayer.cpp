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
	//ベクトル格納先作成
	vector_ = new Vector3();

	//移動制御作成
	movement_ = new FPSPlayerMovement();

	//カメラ作成
	camera_ = Object::Create<FPSCamera>();
	FPSCamera*  FpsCamera = (FPSCamera*)camera_;
	FpsCamera->SetPlayer(this);

	//武器作成
	shotgun_ = Object::Create<Weapon_Shotgun>();
	shotgun_->SetPlayer(this);

	//UI作成
	playerUI_ = new FPSPlayer_UI();
	playerUI_->Init();
}

FPSPlayer::~FPSPlayer()
{
	//ベクトル破棄
	delete vector_;

	//移動制御破棄
	delete movement_;

	//UI破棄
	delete playerUI_;
}

void FPSPlayer::Init()
{
	SetPosition(0.0f,0.0f, 8.0f);
	SetRotation(0.0f,0.0f,0.0f);
	SetScale(1.0f,1.0f,1.0f);

	//プレイヤーUI初期化
	playerUI_->Init();
}

void FPSPlayer::Uninit()
{
	//プレイヤーUI終了処理
	playerUI_->Uninit();
}

void FPSPlayer::Update()
{
	//移動処理更新
	movement_->Act(this);

	//UI更新
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
	//プレイヤーUI描画
	playerUI_->Draw();
}

void FPSPlayer::EndDraw()
{

}

Weapon * FPSPlayer::GetWeapon()
{
	return shotgun_;
}
