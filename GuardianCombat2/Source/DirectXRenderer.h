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

	// �l�p�`�̕`��
	static void DrawQuad(
		float x0, float y0, float x1, float y1,
		float x2, float y2, float x3, float y3,
		float u0, float v0, float u1, float v1,
		D3DCOLOR diffuse
	);
	static void DrawQuad(
		ID3DXEffect* effect,
		float x0, float y0, float x1, float y1,
		float x2, float y2, float x3, float y3,
		float u0, float v0, float u1, float v1,
		D3DCOLOR diffuse
	);
	static void DrawRect(
		ID3DXEffect* effect,
		float x0, float y0, float x1, float y1,
		float u0, float v0, float u1, float v1,
		D3DCOLOR diffuse
	) {
		DrawQuad(
			effect, x0, y0, x1, y0, x0, y1,
			x1, y1, u0, v0, u1, v1, diffuse);
	}
public:
	static LPDIRECT3DDEVICE9 GetDevice();
	static LPDIRECT3DSURFACE9 renderTarget_;
	static LPDIRECT3DSURFACE9 depthStencil_;
	
};