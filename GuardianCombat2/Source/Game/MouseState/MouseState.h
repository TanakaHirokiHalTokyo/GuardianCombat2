#pragma once


class MouseState
{
public:
	static POINT GetMouseCursor();
	static bool IsHitMouseCursorCircle(float mouse_rad,float bx,float by,float br);
	static bool IsHitMouseCursorBox(float bx,float by,float bw,float bh);
private:
	static POINT point_;
};