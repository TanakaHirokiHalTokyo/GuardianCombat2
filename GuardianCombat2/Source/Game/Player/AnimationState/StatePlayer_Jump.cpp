#include "StatePlayer_Jump.h"
#include "../DefaultPlayer.h"
#include "../../SkinMesh/sceneSkinMesh.h"
#include "../../../main.h"
#include "../../../DInput/DirectInput.h"
#include "StatePlayer_Idle.h"
#include "StatePlayer_Walk_Forward.h"
#include "../../../Vector3/Vector3.h"
#include "StatePlayer_Walk_Back.h"

constexpr float ANIMATION_SPEED = 1.5f;
constexpr float ANIMATION_TIME = 3.5f;
constexpr int ANIMATION_COUNT = (const int)(ANIMATION_TIME * GameFPS / ANIMATION_SPEED);

void StatePlayer_Jump::UpdateKeyboard(DefaultPlayer * player)
{
	//Get SkinMeshModel
	auto player_model = player->GetAnimModel();
	//Get PlayerPosition
	const auto& player_position = player->GetPosition();

	player_model->SetAnimPlaySpeed(ANIMATION_SPEED);

	//‹ó’†
	player->SetOnLand(false);

	velocity_ -= 0.0013f;

	if (GetKeyboardPress(WALK_FRONT_KEY))
	{
		D3DXVECTOR3 new_position = player_position + player->GetVector()->GetFront() * PLAYER_DEFAULT_SPEED;
		player->SetPosition(new_position.x, player_position.y + velocity_, new_position.z);
	}
	else if (GetKeyboardPress(WALK_BACK_KEY))
	{
		D3DXVECTOR3 new_position = player_position + player->GetVector()->GetFront() * PLAYER_DEFAULT_SPEED * -1.0f;
		player->SetPosition(new_position.x, player_position.y + velocity_, new_position.z);
	}
	else
	{
		player->SetPosition(player_position.x, player_position.y + velocity_, player_position.z);
	}
	//UpdateCount
	count_++;

	if (count_ > ANIMATION_COUNT)
	{
		player->SetOnLand(true);
		if (GetKeyboardPress(WALK_FRONT_KEY))
		{
			player_model->ChangeAnim(DefaultPlayer::WALK_FORWARD, ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Walk_Forward());
			return;
		}
		if (GetKeyboardPress(WALK_BACK_KEY))
		{
			player_model->ChangeAnim(DefaultPlayer::WALK_BACK, ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Walk_Forward());
			return;
		}

		player_model->ChangeAnim(DefaultPlayer::IDLE,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Idle());
		return;
	}
}
