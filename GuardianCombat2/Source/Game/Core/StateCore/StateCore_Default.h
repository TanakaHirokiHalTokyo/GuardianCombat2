#pragma once
#include "StateCore.h"

class StateCore_UpDown;

class StateCore_Default :public StateCore
{
public:
	StateCore_Default();
	~StateCore_Default()override;
	void UpdateCore(Core* core)override;
	void ChangeMoveState(StateCore_UpDown* state);

private:
	StateCore_UpDown* state_ = nullptr;
};