#pragma once

#include <d3dx9.h>
class ShadowMapShader
{
public:
	static LPD3DXEFFECT GetEffect();
	static void SetInfo(D3DXMATRIX world);
	static void Uninit();
	static LPDIRECT3DTEXTURE9 GetTexture();
	static void CreateShadowMap();
	static void CreateShadowMapTexture();
private:
	static LPD3DXEFFECT effect_;
	static LPDIRECT3DTEXTURE9 ShadowTexture;
	static LPDIRECT3DSURFACE9 ShadowDepthStencil;
	static bool UseIntTexture;

};