#include "WeaponRelative.h"
#include "../Weapon.h"
#include "../../Player.h"
#include "../../../../Camera/Camera.h"
#include "../../../../Imgui/ImguiManager.h"

void WeaponRelative::Act(Weapon * weapon)
{
	//武器の親になっているプレイヤーを取得
	const auto& player = weapon->GetPlayer();

	//プレイヤーのカメラの情報を取得
	const auto& player_camera = player->GetCamera();

	//プレイヤーのワールドを取得
	const auto& player_world = player->GetWorld();

	//位置設定
	weapon->SetPosition(player_camera->GetPosition() + player_camera->GetFront() * 0.15f);
	weapon->SetPosition(weapon->GetPosition() + player_camera->GetRight() * 0.1f + player_camera->GetUp() * -0.05f);
	//回転角度設定
	weapon->SetRotation(player_camera->GetRotate().y,player_camera->GetRotate().x,0);

}
