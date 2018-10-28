#include "StateCore_UpDown.h"
#include "../Core.h"
#include "StateCore_Default.h"

constexpr float UPDOWN_VALUE = 0.001f;
constexpr float UPDOWN_VALUE_FRAME = 0.00001f;

void StateCore_Up::Update(Core * core, StateCore_Default* default_state)
{
	//GetCorePosition
	const auto& core_position = core->GetPosition();

	//UpdateOffset
	offset_ += UPDOWN_VALUE_FRAME;

	//UpdateOffset
	core->SetPosition(core_position.x,core_position.y + UPDOWN_VALUE,core_position.z);

	if (offset_ > UPDOWN_VALUE)
	{
		default_state->ChangeMoveState(new StateCore_Down());
	}

}

void StateCore_Down::Update(Core * core, StateCore_Default* default_state)
{
	//GetCorePosition
	const auto& core_position = core->GetPosition();
	core->SetPosition(core_position.x, core_position.y - UPDOWN_VALUE, core_position.z);

	//UpdateOffset
	offset_ -= UPDOWN_VALUE_FRAME;

	if (offset_ < -UPDOWN_VALUE)
	{
		default_state->ChangeMoveState(new StateCore_Up());
	}
}
