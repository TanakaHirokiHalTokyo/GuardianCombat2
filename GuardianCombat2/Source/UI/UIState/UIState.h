#pragma once
class UI;

class UIState
{
public:
	virtual void Update(UI* ui) = 0;
};