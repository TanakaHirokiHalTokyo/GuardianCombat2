#pragma once
#include "StatePlayer.h"

class StatePlayer_Idle :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player)override;
};