#include <d3d9.h>
#include <d3dx9.h>
#include "../MouseState/MouseState.h"
#include "../main.h"

POINT MouseState::point_ = {};

POINT MouseState::GetMouseCursor()
{
	// 現在のカーソルの位置を取得
	GetCursorPos(&point_);
	ScreenToClient(GethWnd(), &point_);
	RECT LockR = { point_.x, point_.y, point_.x + 1, point_.y + 1 };  // カーソル位置のみをロック

	return point_;
}