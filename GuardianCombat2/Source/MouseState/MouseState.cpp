#include <d3d9.h>
#include <d3dx9.h>
#include "../MouseState/MouseState.h"
#include "../main.h"

POINT MouseState::point_ = {};

POINT MouseState::GetMouseCursor()
{
	// ���݂̃J�[�\���̈ʒu���擾
	GetCursorPos(&point_);
	ScreenToClient(GethWnd(), &point_);
	RECT LockR = { point_.x, point_.y, point_.x + 1, point_.y + 1 };  // �J�[�\���ʒu�݂̂����b�N

	return point_;
}