#include "FPSPlayer_Movement.h"
#include "../FPSPlayer.h"
#include "../../../Camera/Camera.h"
#include "../../../DInput/DirectInput.h"
#include "../../../XInput/xcontroller.h"
#include "../../../Vector3/Vector3.h"
#include "../Weapon/Weapon.h"
#include "../Weapon/Bullet/Bullet.h"
#include "../../MeshField/MeshField.h"
#include "../../../Imgui/ImguiManager.h"

void FPSPlayerMovement::Act(FPSPlayer* player)
{
	//PlayerPosition取得
	const auto& player_position = player->GetPosition();
	//PlayerCamera取得
	const auto& player_camera = player->GetCamera();

	//カメラの情報取得
	const auto& camera_front = player_camera->GetFront();
	const auto& camera_right = player_camera->GetRight();
	const auto& camera_up = player_camera->GetUp();

	//プレイヤー情報に格納
	D3DXVECTOR3 player_front = camera_front;
	D3DXVECTOR3 player_right = camera_right;
	D3DXVECTOR3 player_up = camera_up;

	//プレイヤーの前・右方向ベクトルのY座標を０にして、正規化
	player_front.y = 0;
	D3DXVec3Normalize(&player_front, &player_front);
	player_right.y = 0;
	D3DXVec3Normalize(&player_right, &player_right);

	//ベクトル格納
	auto player_vector = player->GetVector();
	player_vector->SetFront(player_front);
	player_vector->SetRight(player_right);
	player_vector->SetUp(player_up);

	transvector_ = D3DXVECTOR3(0,0,0);

	float controllerLX = (float)X_CONTROLLER::GetXcontrollerLStickX(1);
	float controllerLY = (float)X_CONTROLLER::GetXcontrollerLStickY(1);

	//前方向移動
	if (GetKeyboardPress(DIK_W) || controllerLY > 0)
	{
		transvector_ += player_front;
	}
	//後ろ方向移動
	if (GetKeyboardPress(DIK_S) || controllerLY < 0)
	{
		transvector_ += player_front * -1.0f;
	}
	//右方向移動
	if (GetKeyboardPress(DIK_D) || controllerLX > 0)
	{
		transvector_ += player_right;
	}
	//左方向移動
	if (GetKeyboardPress(DIK_A) || controllerLX < 0)
	{
		transvector_ += player_right * -1.0f;
	}

	D3DXVec3Normalize(&transvector_,&transvector_);
	player->SetPosition(player_position + transvector_ * player->FPSPLAYER_DEF_SPEED);
	player->SetPositionY(MeshField::GetHeight(player->GetPosition()));


}
