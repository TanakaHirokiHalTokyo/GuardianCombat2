#include "StatePlayer_Walk_Back.h"
#include "../DefaultPlayer.h"
#include "../../SkinMesh/sceneSkinMesh.h"
#include "../../../DInput/DirectInput.h"
#include "StatePlayer_Idle.h"
#include "../../../Vector3/Vector3.h"
#include "StatePlayer_Walk_Forward.h"

void StatePlayer_Walk_Back::UpdateKeyboard(DefaultPlayer * player)
{
	//	Get PlayerModel
	auto player_model = player->GetAnimModel();
	// Get PlayerPosition
	const auto& player_position = player->GetPosition();

	player_model->SetAnimPlaySpeed(ANIMATION_SPEED_WALK);

	// PlayerVector 
	const auto& player_vector = player->GetVector();
	D3DXVECTOR3 new_position = player_position + player_vector->GetFront() * -1.0f * PLAYER_DEFAULT_SPEED;
	player->SetPosition(new_position);


	if (!GetKeyboardPress(WALK_BACK_KEY))
	{
		if (GetKeyboardPress(WALK_FRONT_KEY))
		{
			player_model->ChangeAnim(DefaultPlayer::WALK_FORWARD, ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Walk_Forward());
			return;
		}

		player_model->ChangeAnim(DefaultPlayer::IDLE,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Idle());
		return;
	}
	return;
}
