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
void CRendererDirectX::DrawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float u0, float v0, float u1, float v1, D3DCOLOR diffuse)
{
	// ���_�o�b�t�@���f�o�C�X�ɐݒ�
	LPDIRECT3DVERTEXBUFFER9 vb = VertexBuffer;
	pD3DDevice_->SetStreamSource(0, vb, 0, sizeof(VERTEX));
	pD3DDevice_->SetFVF(D3DFVF_VERTEX);

	// ���_�o�b�t�@�̃��b�N
	VERTEX* v;
	if (SUCCEEDED(
		vb->Lock(0, 4 * sizeof(VERTEX), (void**)&v, 0)
	)) {
		// ���_���W�̐ݒ�
		v[0].X = x0; v[0].Y = y0;
		v[1].X = x1; v[1].Y = y1;
		v[2].X = x2; v[2].Y = y2;
		v[3].X = x3; v[3].Y = y3;

		// �e�N�X�`�����W�̐ݒ�
		v[0].U0 = v[0].U1 = u0; v[0].V0 = v[0].V1 = v0;
		v[1].U0 = v[1].U1 = u1; v[1].V0 = v[1].V1 = v0;
		v[2].U0 = v[2].U1 = u0; v[2].V0 = v[2].V1 = v1;
		v[3].U0 = v[3].U1 = u1; v[3].V0 = v[3].V1 = v1;

		// Z, RHW, ���_�F�̐ݒ�
		for (int i = 0; i < 4; i++) {
			v[i].Z = 0;
			v[i].RHW = 1;
			v[i].Diffuse = diffuse;
		}

		// ���_�o�b�t�@�̉��
		vb->Unlock();

		// �����_�����O�����̐ݒ�
		pD3DDevice_->SetRenderState(D3DRS_ZENABLE, FALSE);
		pD3DDevice_->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// �v���~�e�B�u�̕`��
		pD3DDevice_->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// �����_�����O�����̕��A
		pD3DDevice_->SetRenderState(D3DRS_ZENABLE, TRUE);
		pD3DDevice_->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}
void CRendererDirectX::DrawQuad(ID3DXEffect * effect, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float u0, float v0, float u1, float v1, D3DCOLOR diffuse)
{
	UINT p, np;
	effect->Begin(&np, 0);
	for (p = 0; p<np; p++) {
		effect->BeginPass(p);
		DrawQuad(x0, y0, x1, y1, x2, y2, x3, y3, u0, v0, u1, v1, diffuse);
		effect->EndPass();
	}
	effect->End();
}
LPDIRECT3DDEVICE9 CRendererDirectX::GetDevice()
{
	return pD3DDevice_;
}
