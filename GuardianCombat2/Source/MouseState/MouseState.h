#pragma once
#include "../DInput/DirectInput.h"


class MouseState
{
public:
	static POINT GetMouseCursor();

private:
	static POINT point_;
};