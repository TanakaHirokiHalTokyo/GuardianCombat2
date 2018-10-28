#include "StatePlayer_PunchCombo.h"
#include "StatePlayer_Idle.h"
#include "../DefaultPlayer.h"
#include "../../SkinMesh/sceneSkinMesh.h"
#include "../../../DInput/DirectInput.h"
#include "../../../main.h"
#include "StatePlayer_Walk_Back.h"
#include "StatePlayer_Walk_Forward.h"

constexpr float ANIMATION_SPEED = 2.0f;
constexpr float ANIMATION_TIME = 1.5f;
constexpr int ANIMATION_COUNT = (const int)(ANIMATION_TIME * GameFPS / ANIMATION_SPEED);

void StatePlayer_PunchCombo::UpdateKeyboard(DefaultPlayer * player)
{
	//Get SkinMeshModel
	auto player_model = player->GetAnimModel();

	player_model->SetAnimPlaySpeed(ANIMATION_SPEED);

	//UpdateCount
	count_++;

	if (count_ > ANIMATION_COUNT)
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

		player_model->ChangeAnim(DefaultPlayer::IDLE,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Idle());
		return;
	}
}
