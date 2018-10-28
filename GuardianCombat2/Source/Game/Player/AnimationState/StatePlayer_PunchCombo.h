#pragma once
#include "StatePlayer.h"

class StatePlayer_PunchCombo :public StatePlayer
{
public:
	void UpdateKeyboard(DefaultPlayer* player)override;
private:
	int count_ = 0;
};