#pragma once
#include "StatePlayer.h"

class StatePlayer_Walk_Back :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player)override;
};