#pragma once
#include "StatePlayer.h"

class StatePlayer_PunchLeft :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player)override;
private:
	bool combo_ = false;
	int count_ = 0;
};