#pragma once
#include "StatePlayer.h"

class StatePlayer_PunchRight :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player)override;
private:
	int count_ = 0;
	bool combo_ = false;
};
