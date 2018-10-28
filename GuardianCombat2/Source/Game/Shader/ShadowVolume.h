#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class ShadowVolume
{
public:
	static LPD3DXEFFECT GetEffect();
	static void SetInfo(D3DXMATRIX world);
private:
	static LPD3DXEFFECT effect_;
};