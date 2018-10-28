#include "StatePlayer_Walk_Forward.h"
#include "../DefaultPlayer.h"
#include "../../SkinMesh/sceneSkinMesh.h"
#include "../../../DInput/DirectInput.h"
#include "StatePlayer_Idle.h"
#include "StatePlayer_Run.h"
#include "../../../Vector3/Vector3.h"
#include "StatePlayer_Walk_Back.h"


void StatePlayer_Walk_Forward::UpdateKeyboard(DefaultPlayer * player)
{
	//Get SkinMeshModel
	auto player_model = player->GetAnimModel();
	//Get PlayerPosition
	const auto& player_position = player->GetPosition();
	// AnimationSpeed Set
	player_model->SetAnimPlaySpeed(ANIMATION_SPEED_WALK);
	// PlayerVector 
	const auto& player_vector = player->GetVector();

	D3DXVECTOR3 new_position = player_position + player_vector->GetFront() * PLAYER_DEFAULT_SPEED;
	player->SetPosition(new_position);

	//  •à‚¢‚Ä‚¢‚È‚¢‚Î‚ ‚¢
	if (!GetKeyboardPress(WALK_FRONT_KEY))
	{
		if (GetKeyboardPress(WALK_BACK_KEY))
		{
			player_model->ChangeAnim(DefaultPlayer::WALK_BACK, ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Walk_Back());
			return;
		}

		//Idle Motion
		player_model->ChangeAnim(DefaultPlayer::IDLE,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Idle());
		return;
	}

	//‘–‚è
	if (GetKeyboardTrigger(SPRINT_KEY))
	{
		player_model->ChangeAnim(DefaultPlayer::RUN, ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Run());
		return;
	}
}
