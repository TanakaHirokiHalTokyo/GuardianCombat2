#include "StatePlayer_Idle.h"
#include "../../../DInput/DirectInput.h"
#include "../DefaultPlayer.h"
#include "../../SkinMesh/sceneSkinMesh.h"
#include "StatePlayer_Walk_Forward.h"
#include "StatePlayer_Walk_Back.h"
#include "StatePlayer_KickRight.h"
#include "StatePlayer_PunchLeft.h"
#include "StatePlayer_Run.h"
#include "StatePlayer_Jump.h"

void StatePlayer_Idle::UpdateKeyboard(DefaultPlayer* player)
{
	//Get SkinMesh
	auto player_model = player->GetAnimModel();

	player_model->SetAnimPlaySpeed(ANIMATION_SPEED_IDLE);

	//�L�b�N
	if (GetKeyboardTrigger(KICK_KEY))
	{
		player_model->ChangeAnim(DefaultPlayer::KICK_RIGHT,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_KickRight());
		return;
	}
	//�p���`
	if (GetKeyboardTrigger(PUNCH_KEY))
	{
		player_model->ChangeAnim(DefaultPlayer::PUNCH_LEFT,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_PunchLeft());
		return;
	}
	if (GetKeyboardTrigger(JUMP_KEY))
	{
		player_model->ChangeAnim(DefaultPlayer::JUMP, ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Jump());
		return;
	}

	//		�O����
	if (GetKeyboardTrigger(WALK_FRONT_KEY))
	{
		//�@�@����
		if (GetKeyboardPress(SPRINT_KEY))
		{
			player_model->ChangeAnim(DefaultPlayer::RUN, ANIMATION_SHIFTTIME);
			player->ChangeState(new StatePlayer_Run());
			return;
		}

		player_model->ChangeAnim(DefaultPlayer::WALK_FORWARD,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Walk_Forward());
		return;
	}
	//		������
	if (GetKeyboardTrigger(WALK_BACK_KEY))
	{
		player_model->ChangeAnim(DefaultPlayer::WALK_BACK,ANIMATION_SHIFTTIME);
		player->ChangeState(new StatePlayer_Walk_Back());
		return;
	}


	return;
}
