#pragma once

#include "FPSPlayer_State.h"
#include <d3dx9.h>
#include <d3d9.h> 

class FPSPlayerMovement:public FPSPlayerState
{
public:
	void Act(FPSPlayer* player)override;		//移動実行関数
private:
	D3DXVECTOR3 transvector_ = D3DXVECTOR3(0,0,0);
};