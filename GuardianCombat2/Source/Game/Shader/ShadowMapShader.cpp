#include "ShadowMapShader.h"
#include "ShaderManager.h"
#include "../../Camera/Camera.h"
#include "../../Light/Light.h"
#include "../../Texture/Texture.h"
#include "../../Game/GameManager/GameManager.h"
#include "../../DirectXRenderer.h"

#define TEX_SIZE 4096

LPD3DXEFFECT ShadowMapShader::effect_ = NULL;
LPDIRECT3DTEXTURE9 ShadowMapShader::ShadowTexture = NULL;
LPDIRECT3DSURFACE9 ShadowMapShader::ShadowDepthStencil = NULL;
bool ShadowMapShader::UseIntTexture = false;

LPD3DXEFFECT ShadowMapShader::GetEffect()
{
	if (effect_ == NULL)
	{
		effect_ = ShaderManager::GetEffect(ShaderManager::Shader_ShadowMap);
	}
	return effect_;
}

void ShadowMapShader::SetInfo(D3DXMATRIX world)
{
	if (effect_ == NULL)
	{
		effect_ = ShaderManager::GetEffect(ShaderManager::Shader_ShadowMap);
	}
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	// �V���h�E�}�b�v�̍쐬
	if (!ShadowTexture) {

		// ���������e�N�X�`��
		UseIntTexture = false;
		pDevice->CreateTexture(
			TEX_SIZE, TEX_SIZE, 1,
			D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
			D3DPOOL_DEFAULT, &ShadowTexture, NULL);

		if (!ShadowTexture) {
			pDevice->CreateTexture(
				TEX_SIZE, TEX_SIZE, 1,
				D3DUSAGE_RENDERTARGET, D3DFMT_R16F,
				D3DPOOL_DEFAULT, &ShadowTexture, NULL);

			if (!ShadowTexture) {
				pDevice->CreateTexture(
					TEX_SIZE, TEX_SIZE, 1,
					D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F,
					D3DPOOL_DEFAULT, &ShadowTexture, NULL);

				// �����e�N�X�`��
				if (!ShadowTexture) {
					UseIntTexture = true;
					pDevice->CreateTexture(
						TEX_SIZE, TEX_SIZE, 1,
						D3DUSAGE_RENDERTARGET, D3DFMT_G16R16,
						D3DPOOL_DEFAULT, &ShadowTexture, NULL);
					if (!ShadowTexture) {
						pDevice->CreateTexture(
							TEX_SIZE, TEX_SIZE, 1,
							D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16,
							D3DPOOL_DEFAULT, &ShadowTexture, NULL);
					} 
				}
			}
		}
	}
	// �[�x�o�b�t�@�̍쐬
	if (!ShadowDepthStencil) {
		pDevice->CreateDepthStencilSurface(TEX_SIZE, TEX_SIZE, D3DFMT_D16,D3DMULTISAMPLE_NONE, 0, TRUE,&ShadowDepthStencil, NULL);
	} 

	//Camera ���
	CAMERA_INFO& camInfo = Camera::GetCameraInfo();
	D3DXMATRIX wv = world * camInfo.view;
	D3DXMATRIX wvp = world * camInfo.view * camInfo.proj;	//���[���h�r���[�v���W�F�N�V����
	DirectionalLight* light = GameManager::GetDirectionalLight();
	D3DXVECTOR4 Ambient = light->GetAmbient();

	effect_->SetMatrix("World",&world);		//���[���h�Z�b�g
	effect_->SetMatrix("WView",&wv);	//View�Z�b�g
	effect_->SetMatrix("WVProj",&wvp);
	
	//���C�g�̐ݒ�
	const auto lightPos = D3DXVECTOR3(0.0f,30.0f, -15.0f);
	const auto lightAt = D3DXVECTOR3(0, 0, 0);
	const auto lightUp = D3DXVECTOR3(0,1.0f,0);

	effect_->SetValue("LightWPos",&lightPos,sizeof(D3DXVECTOR3));

	//���C�g���猩���Ƃ��̃r���[�s��
	D3DXMATRIX lightView;
	D3DXMatrixLookAtLH(&lightView,&lightPos,&lightAt,&lightUp);

	//���C�g���猩���Ƃ��̃r���[�ˉe�s��
	D3DXMATRIX lightVProj;
	D3DXMatrixPerspectiveLH(&lightVProj,1,1,1,5000);
	D3DXMatrixMultiply(&lightVProj,&lightView,&lightVProj);
	effect_->SetMatrix("LightVProj",&lightVProj);

	

	effect_->SetFloat("Margin", 0.0001f);
	if (UseIntTexture)
	{
		effect_->SetFloat("Margin", 0.0002f);
	}
	effect_->SetVector("Ambient",&light->GetAmbient());
}

void ShadowMapShader::Uninit()
{
	SAFE_RELEASE(ShadowTexture);
	SAFE_RELEASE(ShadowDepthStencil);
}

LPDIRECT3DTEXTURE9 ShadowMapShader::GetTexture()
{
	return ShadowTexture;
}

void ShadowMapShader::CreateShadowMap()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	//CRendererDirectX::Clear();
	//�V���h�E�}�b�v�̍쐬
	LPDIRECT3DSURFACE9 rt;
	if (SUCCEEDED(ShadowTexture->GetSurfaceLevel(0, &rt))) {
		pDevice->SetRenderTarget(0, rt);
		rt->Release();
		pDevice->SetDepthStencilSurface(ShadowDepthStencil);
	}
	effect_->SetVector("LightColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	effect_->SetTexture("ShadowTex", ShadowTexture);
}

void ShadowMapShader::CreateShadowMapTexture()
{
	if (effect_ == NULL)
	{
		effect_ = ShaderManager::GetEffect(ShaderManager::Shader_ShadowMap);
	}

	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	// �V���h�E�}�b�v�̍쐬
	if (!ShadowTexture) {

		// ���������e�N�X�`��
		UseIntTexture = false;
		pDevice->CreateTexture(
			TEX_SIZE, TEX_SIZE, 1,
			D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
			D3DPOOL_DEFAULT, &ShadowTexture, NULL);

		if (!ShadowTexture) {
			pDevice->CreateTexture(
				TEX_SIZE, TEX_SIZE, 1,
				D3DUSAGE_RENDERTARGET, D3DFMT_R16F,
				D3DPOOL_DEFAULT, &ShadowTexture, NULL);

			if (!ShadowTexture) {
				pDevice->CreateTexture(
					TEX_SIZE, TEX_SIZE, 1,
					D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F,
					D3DPOOL_DEFAULT, &ShadowTexture, NULL);

				// �����e�N�X�`��
				if (!ShadowTexture) {
					UseIntTexture = true;
					pDevice->CreateTexture(
						TEX_SIZE, TEX_SIZE, 1,
						D3DUSAGE_RENDERTARGET, D3DFMT_G16R16,
						D3DPOOL_DEFAULT, &ShadowTexture, NULL);
					if (!ShadowTexture) {
						pDevice->CreateTexture(
							TEX_SIZE, TEX_SIZE, 1,
							D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16,
							D3DPOOL_DEFAULT, &ShadowTexture, NULL);
					}
				}
			}
		}
	}
	// �[�x�o�b�t�@�̍쐬
	if (!ShadowDepthStencil) {
		pDevice->CreateDepthStencilSurface(TEX_SIZE, TEX_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &ShadowDepthStencil, NULL);
	}
}
