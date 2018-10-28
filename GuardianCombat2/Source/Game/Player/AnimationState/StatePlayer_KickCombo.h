#pragma once
#include "StatePlayer.h"

class StatePlayer_KickCombo :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player);

private:
	int count_ = 0;
};