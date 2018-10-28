#pragma once
#include "StatePlayer.h"

class StatePlayer_Jump :public StatePlayer
{
public:
	const float JUMP_VALUE = 0.1f;

	void UpdateKeyboard(DefaultPlayer* player)override;
private:
	int count_ = 0;
	float velocity_ = JUMP_VALUE;
};