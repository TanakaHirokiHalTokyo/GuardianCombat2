#pragma once
#include "StatePlayer.h"

class StatePlayer_Run :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player)override;
};