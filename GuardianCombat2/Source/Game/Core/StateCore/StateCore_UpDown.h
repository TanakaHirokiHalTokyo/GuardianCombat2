#pragma once

class Core;
class StateCore_Default;

class StateCore_UpDown
{
public:
	virtual void Update(Core* core, StateCore_Default* default_state) = 0;
protected:
	float offset_ = 0.0f;
};

//��ɓ�����
class StateCore_Up:public StateCore_UpDown
{
public:
	void Update(Core* core, StateCore_Default* default_state)override;
};

//���ɓ�����
class StateCore_Down :public StateCore_UpDown
{
public:
	void Update(Core* core, StateCore_Default* default_state)override;
};