#include "FPSPlayer.h"
#include "../../Camera/FPSCamera.h"
#include "../../Vector3/Vector3.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"
#include "../../Imgui/ImguiManager.h"
#include "StatePlayer\FPSPlayer_Movement.h"

FPSPlayer::FPSPlayer()
{
	//ベクトル格納先作成
	vector_ = new Vector3();

	//移動制御作成
	movement_ = new FPSPlayerMovement();

	//カメラ作成
	camera_ = new FPSCamera();
	FPSCamera*  FpsCamera = (FPSCamera*)camera_;
	FpsCamera->SetPlayer(this);
}

FPSPlayer::~FPSPlayer()
{
	//ベクトル破棄
	delete vector_;

	//移動制御破棄
	delete movement_;

	//カメラ破棄
	camera_->Uninit();
	delete camera_;
	camera_ = nullptr;
}

void FPSPlayer::Init()
{
	SetPosition(0.0f,0.0f, 8.0f);
	SetRotation(0.0f,0.0f,0.0f);
	SetScale(1.0f,1.0f,1.0f);

	//カメラ初期化
	camera_->Init();
}

void FPSPlayer::Uninit()
{
	//カメラ終了処理
	camera_->Uninit();
}

void FPSPlayer::Update()
{
	//移動処理更新
	movement_->Act(this);

	//カメラ更新
	camera_->Update();
}

void FPSPlayer::BeginDraw()
{
	camera_->BeginDraw();
}

void FPSPlayer::Draw()
{
	//カメラ描画処理
	camera_->Draw();
}

void FPSPlayer::EndDraw()
{
	//カメラ描画終了時処理
	camera_->EndDraw();
}

Camera * FPSPlayer::GetCamera()
{
	return camera_;
}	
