#pragma once
class UI;

class UIState
{
public:
	virtual void Init() = 0;
	virtual void Update(UI* ui) = 0;
};