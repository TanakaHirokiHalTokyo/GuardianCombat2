#include "main.h"
#include "DirectXRenderer.h"

// FVF
#define D3DFVF_VERTEX \
	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX2|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1))


LPDIRECT3D9 CRendererDirectX::pD3D_ = NULL;
LPDIRECT3DDEVICE9 CRendererDirectX::pD3DDevice_ = NULL;
D3DPRESENT_PARAMETERS CRendererDirectX::d3dpp_;
D3DDISPLAYMODE	CRendererDirectX::d3ddm_;
LPDIRECT3DSURFACE9 CRendererDirectX::renderTarget_ = NULL;
LPDIRECT3DSURFACE9 CRendererDirectX::depthStencil_ = NULL;
LPDIRECT3DVERTEXBUFFER9 CRendererDirectX::VertexBuffer = NULL;
LPDIRECT3DTEXTURE9 CRendererDirectX::blurTexture1 = nullptr;				//�u���[�e�N�X�`���P
LPDIRECT3DSURFACE9 CRendererDirectX::blurSurface1 = nullptr;				//�u���[�T�[�t�F�C�X�P
LPDIRECT3DTEXTURE9 CRendererDirectX::blurTexture2 = nullptr;				//�u���[�e�N�X�`���Q
LPDIRECT3DSURFACE9 CRendererDirectX::blurSurface2 = nullptr;				//�u���[�T�[�t�F�C�X�Q

BOOL CRendererDirectX::Init(HWND hWnd, BOOL bWindow)
{

	//Direct3D�I�u�W�F�N�g�̍쐬
	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D_ == NULL)
	{
		return E_FAIL;
	}

	if (FAILED(pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm_)))	//���s������^��Ԃ�
	{
		return E_FAIL;
	}

	ZeroMemory(&d3dpp_, sizeof(d3dpp_));								//�������������� d3dpp�̍\���̂�0�ŏ�����
	d3dpp_.BackBufferWidth = (UINT)ScreenWidth;							//�o�b�N�o�b�t�@�̕�
	d3dpp_.BackBufferHeight = (UINT)ScreenHeight;							//				����
	d3dpp_.BackBufferFormat = d3ddm_.Format;							//�f�B�X�v���C���[�h�̃t�H�[�}�b�g��ݒ�
	d3dpp_.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp_.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�������������̂�	DISCARD�ɂ��Ă����΂���
	d3dpp_.EnableAutoDepthStencil = TRUE;							//3�����̂��̂����̂ŕK��TRUE
	d3dpp_.AutoDepthStencilFormat = D3DFMT_D16;						//
	d3dpp_.Windowed = bWindow;										//TRUE:Window���[�h		FALSE:FULLSCREEN
	d3dpp_.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//FullScreen���̃��t���b�V�����[�g
	d3dpp_.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�����������݂čX�V

																	//�f�o�C�X�I�u�W�F�N�g�̐���
																	//[�f�o�C�X�쐬����0]<�`��>��<���_����>��
	if (FAILED(pD3D_->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp_,
		&pD3DDevice_
	)))
	{
		//��L�̐ݒ肪���s������
		//[�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A
		MessageBoxA(hWnd, "�f�o�C�X�����Ɏ��s���܂����B", "ERROR", MB_OK);
		return false;
	};
	D3DXCreateTexture(pD3DDevice_, ScreenWidth, ScreenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &blurTexture1);
	blurTexture1->GetSurfaceLevel(0, &blurSurface1);

	D3DXCreateTexture(pD3DDevice_, ScreenWidth, ScreenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &blurTexture2);
	blurTexture2->GetSurfaceLevel(0, &blurSurface2);

	pD3DDevice_->GetRenderTarget(0,&renderTarget_);
	pD3DDevice_->GetDepthStencilSurface(&depthStencil_);

	// ���_�o�b�t�@�̍쐬
	pD3DDevice_->CreateVertexBuffer(
		4 * sizeof(VERTEX), 0, D3DFVF_VERTEX,
		D3DPOOL_MANAGED, &VertexBuffer, NULL);
	return true;
}
void CRendererDirectX::Uninit()
{
	SAFE_RELEASE(pD3DDevice_);
	SAFE_RELEASE(pD3D_);
	SAFE_RELEASE(renderTarget_);
	SAFE_RELEASE(depthStencil_);
}
void CRendererDirectX::DrawBegin()
{
	pD3DDevice_->Clear(
		0,												//�S��ʃN���A
		NULL,											//�S��ʃN���A
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),			//�N���A�����׸� TARGET:�F�@
		D3DCOLOR_RGBA(100, 100, 100, 255),				//�N���A�J���[ TARGET������邱�ƂŗL���ɂȂ�
		1.0f,											//ZBUFFER�̃N���A�l
		0												//�X�e���V���̃N���A���l	���̓X�e���V�����׸ނ��ĂĂȂ��̂Ŗ��������
	);

	pD3DDevice_->BeginScene();

}
void CRendererDirectX::DrawEnd()
{
	pD3DDevice_->EndScene();							//Direct3D�ɂ��`��̏I��

	pD3DDevice_->Present(
		NULL,
		NULL,
		NULL,											//�ΏۂƂȂ�E�B���h�E�n���h��
		NULL
	);
}
void CRendererDirectX::Clear()
{
	pD3DDevice_->Clear(
		0,												//�S��ʃN���A
		NULL,											//�S��ʃN���A
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),			//�N���A�����׸� TARGET:�F�@
		D3DCOLOR_RGBA(255, 255, 255, 255),				//�N���A�J���[ TARGET������邱�ƂŗL���ɂȂ�
		1.0f,											//ZBUFFER�̃N���A�l
		0												//�X�e���V���̃N���A���l	���̓X�e���V�����׸ނ��ĂĂȂ��̂Ŗ��������
	);
}
void CRendererDirectX::ClearZ()
{
	pD3DDevice_->Clear(
		0,												//�S��ʃN���A
		NULL,											//�S��ʃN���A
		D3DCLEAR_ZBUFFER,			//�N���A�����׸� TARGET:�F�@
		D3DCOLOR_RGBA(255, 255, 255, 255),				//�N���A�J���[ TARGET������邱�ƂŗL���ɂȂ�
		1.0f,											//ZBUFFER�̃N���A�l
		0												//�X�e���V���̃N���A���l	���̓X�e���V�����׸ނ��ĂĂȂ��̂Ŗ��������
	);
}
void CRendererDirectX::ResetRenderTarget()
{
	pD3DDevice_->SetRenderTarget(0,renderTarget_);
	pD3DDevice_->SetDepthStencilSurface(depthStencil_);
}
LPDIRECT3DDEVICE9 CRendererDirectX::GetDevice()
{
	return pD3DDevice_;
}

LPDIRECT3DTEXTURE9 CRendererDirectX::GetBlurTexture1()
{
	return blurTexture1;
}
LPDIRECT3DTEXTURE9 CRendererDirectX::GetBlurTexture2()
{
	return blurTexture2;
}
LPDIRECT3DSURFACE9 CRendererDirectX::GetBlurSurface1()
{
	return blurSurface1;
}
LPDIRECT3DSURFACE9 CRendererDirectX::GetBlurSurface2()
{
	return blurSurface2;
}
LPDIRECT3DSURFACE9 CRendererDirectX::GetBackBuffer()
{
	return renderTarget_;
}
void CRendererDirectX::ReplaceBlur()
{
	LPDIRECT3DTEXTURE9 texture;
	texture = blurTexture1;
	blurTexture1 = blurTexture2;
	blurTexture2 = texture;

	LPDIRECT3DSURFACE9 surface;
	surface = blurSurface1;
	blurSurface1 = blurSurface2;
	blurSurface2 = surface;
}