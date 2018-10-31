#pragma once


class MouseState
{
public:
	static POINT GetMouseCursor();

private:
	static POINT point_;
};