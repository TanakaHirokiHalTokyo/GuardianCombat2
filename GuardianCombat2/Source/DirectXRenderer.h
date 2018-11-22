#pragma once

#include "main.h"

class CRendererDirectX
{
private:
	static LPDIRECT3D9			pD3D_;				//Direct3D�C���^�[�t�F�C�X
	static LPDIRECT3DDEVICE9	pD3DDevice_;		//Direct3D�f�o�C�X
	static D3DPRESENT_PARAMETERS d3dpp_;			//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̍쐬
	static D3DDISPLAYMODE		d3ddm_;				//���݂̃f�B�X�v���C���[�h���擾	
	static LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	static LPDIRECT3DSURFACE9 renderTarget_;
	static LPDIRECT3DSURFACE9 depthStencil_;
	static LPDIRECT3DTEXTURE9 blurTexture1;				//�u���[�e�N�X�`���P
	static LPDIRECT3DSURFACE9 blurSurface1;				//�u���[�T�[�t�F�C�X�P
	static LPDIRECT3DTEXTURE9 blurTexture2;				//�u���[�e�N�X�`���Q
	static LPDIRECT3DSURFACE9 blurSurface2;				//�u���[�T�[�t�F�C�X�Q

public:
	// �\����
	struct VERTEX
	{
		// �ϊ��ςݒ��_
		FLOAT X, Y, Z, RHW;

		// �f�B�t���[�Y�F
		D3DCOLOR Diffuse;

		// �e�N�X�`�����W
		FLOAT U0, V0;
		FLOAT U1, V1;
	};

	static BOOL Init(HWND hWnd, BOOL bWindow);
	static void Uninit();
	static void DrawBegin();
	static void DrawEnd();
	static void Clear();
	static void ClearZ();
	static void ResetRenderTarget();
	static LPDIRECT3DDEVICE9 GetDevice();
	static LPDIRECT3DTEXTURE9 GetBlurTexture1();
	static LPDIRECT3DTEXTURE9 GetBlurTexture2();
	static LPDIRECT3DSURFACE9 GetBlurSurface1();
	static LPDIRECT3DSURFACE9 GetBlurSurface2();
	static LPDIRECT3DSURFACE9 GetBackBuffer();
	static void ReplaceBlur();
	
	
};