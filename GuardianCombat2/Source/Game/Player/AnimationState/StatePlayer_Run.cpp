#include "StatePlayer_Run.h"
#include "../DefaultPlayer.h"
#include "../../SkinMesh/sceneSkinMesh.h"
#include "../../../DInput/DirectInput.h"
#include "StatePlayer_Walk_Forward.h"
#include "StatePlayer_Walk_Back.h"
#include "StatePlayer_Idle.h"
#include "../../../Vector3/Vector3.h"

constexpr float PLAYER_RUN_SPEED = PLAYER_DEFAULT_SPEED * 4.0f;

void StatePlayer_Run::UpdateKeyboard(DefaultPlayer * player)
{
	//Get PlayerModel
	auto player_model = player->GetAnimModel();
	//Get PlayerPosition
	const auto& player_position = player->GetPosition();

	player_model->SetAnimPlaySpeed(ANIMATION_SPEED_RUN);

	//Set Position
	player->SetPosition(player_position + player->GetVector()->GetFront() * PLAYER_RUN_SPEED);

	if (!GetKeyboardPress(SPRINT_KEY))
	{
		if (GetKeyboardPress(WALK_FRONT_KEY))
		{
			player_model->ChangeAnim(DefaultPlayer::WALK_FORWARD,ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Walk_Forward());
			return;
		}
		if (GetKeyboardPress(WALK_BACK_KEY))
		{
			player_model->ChangeAnim(DefaultPlayer::WALK_BACK,ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Walk_Back());
			return;
		}

		//‚È‚É‚à‘€ì‚ð‚µ‚Ä‚¢‚È‚¢‚Æ‚«
		player_model->ChangeAnim(DefaultPlayer::IDLE,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Idle());
		return;
	}
}
