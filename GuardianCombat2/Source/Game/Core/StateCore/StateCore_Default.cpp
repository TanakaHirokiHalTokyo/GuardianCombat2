#include "StateCore_Default.h"
#include "../Core.h"
#include "StateCore_UpDown.h"

StateCore_Default::StateCore_Default()
{
	state_ = new StateCore_Up();
}

StateCore_Default::~StateCore_Default()
{
	delete state_;	
	state_ = nullptr;
}

void StateCore_Default::UpdateCore(Core * core)
{
	if (state_ != nullptr)
	{
		state_->Update(core,this);
	}
}

void StateCore_Default::ChangeMoveState(StateCore_UpDown * state)
{
	if (state_ != nullptr)
	{
		delete state_;
	}
	state_ = state;
}
