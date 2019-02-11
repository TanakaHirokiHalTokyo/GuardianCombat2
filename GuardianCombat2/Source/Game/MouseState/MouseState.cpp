#include <d3d9.h>
#include <d3dx9.h>
#include "../MouseState/MouseState.h"
#include "../../main.h"

POINT MouseState::point_ = {};

POINT MouseState::GetMouseCursor()
{
	// ���݂̃J�[�\���̈ʒu���擾
	GetCursorPos(&point_);
	ScreenToClient(GethWnd(), &point_);
	RECT LockR = { point_.x, point_.y, point_.x + 1, point_.y + 1 };  // �J�[�\���ʒu�݂̂����b�N

	return point_;
}

bool MouseState::IsHitMouseCursorCircle(float mouse_rad, float bx, float by, float br)
{
	// ���݂̃J�[�\���̈ʒu���擾
	GetCursorPos(&point_);
	ScreenToClient(GethWnd(), &point_);
	RECT LockR = { point_.x, point_.y, point_.x + 1, point_.y + 1 };  // �J�[�\���ʒu�݂̂����b�N

	float x = bx - point_.x;
	float y = by - point_.y;

	float l = x * x + y * y;

	return l < (mouse_rad + br) * (mouse_rad * br);
}

bool MouseState::IsHitMouseCursorBox(float bx, float by, float bw, float bh)
{
	// ���݂̃J�[�\���̈ʒu���擾
	GetCursorPos(&point_);
	ScreenToClient(GethWnd(), &point_);
	RECT LockR = { point_.x, point_.y, point_.x + 1, point_.y + 1 };  // �J�[�\���ʒu�݂̂����b�N

	if (point_.x >= bx && point_.x <= bx + bw)
	{
		if (point_.y >= by && point_.y <= by + bh)
		{
			return true;
		}
	}

	return false;
}
