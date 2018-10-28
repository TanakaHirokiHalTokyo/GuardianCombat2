#pragma once
#include "UIState.h"
#include <d3dx9.h>
#include <d3d9.h>

class UIState_Scale :public UIState
{
public:
	void Update(UI* ui)override;

private:
	int count_ = 0;
	D3DXVECTOR2 moveValue_ = D3DXVECTOR2(0, 0);
	bool initScaleValue_ = false;
	bool updateScale_ = true;
};