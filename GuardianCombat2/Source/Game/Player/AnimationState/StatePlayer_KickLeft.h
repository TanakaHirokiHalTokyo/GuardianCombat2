#pragma once
#include "StatePlayer.h"

class StatePlayer_KickLeft :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player);
private:
	int count_ = 0;
	bool combo_ = false;
};