#include "StatePlayer_KickRight.h"
#include "StatePlayer_Idle.h"
#include "../DefaultPlayer.h"
#include "../../SkinMesh/sceneSkinMesh.h"
#include "../../../DInput/DirectInput.h"
#include "../../../main.h"
#include "StatePlayer_KickLeft.h"
#include "StatePlayer_Walk_Back.h"
#include "StatePlayer_Walk_Forward.h"

constexpr float ANIMATION_TIME = 1.8f;
constexpr int	ANIMATION_COUNT = (int)(GameFPS * ANIMATION_TIME / ANIMATION_SPEED_KICK);

void StatePlayer_KickRight::UpdateKeyboard(DefaultPlayer * player)
{
	//Get SkinMeshModel
	auto player_model = player->GetAnimModel();
	
	//count Update
	count_++;

	player_model->SetAnimPlaySpeed(ANIMATION_SPEED_KICK);

	if (count_ > ANIMATION_COUNT)
	{
		if (combo_)
		{
			player_model->ChangeAnim(DefaultPlayer::KICK_LEFT,ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_KickLeft());
			return;
		}
		else
		{
			if (GetKeyboardPress(WALK_FRONT_KEY))
			{
				player_model->ChangeAnim(DefaultPlayer::WALK_FORWARD, ANIMATION_SHIFTTIME);
				player->ChangeState(new StatePlayer_Walk_Forward());
				return;
			}
			if (GetKeyboardPress(WALK_BACK_KEY))
			{
				player_model->ChangeAnim(DefaultPlayer::WALK_BACK, ANIMATION_SHIFTTIME);
				player->ChangeState(new StatePlayer_Walk_Back());
				return;
			}
			player_model->ChangeAnim(DefaultPlayer::IDLE, ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Idle());
			return;
		}
	}
	else
	{
		if (GetKeyboardTrigger(KICK_KEY))
		{
			combo_ = true;
		}
	}
	return;
}
