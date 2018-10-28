#pragma once
#include "StatePlayer.h"

class StatePlayer_Walk_Forward :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player)override;
};