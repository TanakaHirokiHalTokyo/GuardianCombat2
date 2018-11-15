#include "ToonShader.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "../../Camera/Camera.h"
#include "../../Light/Light.h"
#include "../../Texture/Texture.h"
#include "../../Game/GameManager/GameManager.h"
#include "ShaderManager.h"

LPD3DXEFFECT ToonShader::toon_ = NULL;


LPD3DXEFFECT ToonShader::GetEffect()
{
	if (toon_ == NULL)
	{
		toon_ = ShaderManager::GetEffect(ShaderManager::Shader_Toon);
	}

	return toon_;
}

void ToonShader::SetInfo(D3DXMATRIX world)
{
	if (toon_ == NULL)
	{
		toon_ = ShaderManager::GetEffect(ShaderManager::Shader_Toon);
	}

	//Camera ���
	CAMERA_INFO camInfo = Camera::GetCameraInfo();

	D3DXMATRIX& World = world;	//���[���h���W
	D3DXMATRIX wvp = world * camInfo.view * camInfo.proj;	//���[���h�r���[�v���W�F�N�V����
	DirectionalLight* light = GameManager::GetDirectionalLight();
	D3DXVECTOR3 lightVec = light->GetVector();
	D3DXVECTOR4 Ambient = light->GetAmbient();
	LPDIRECT3DTEXTURE9 toonTex = TextureManager::GetTexture(TextureManager::Tex_Toon).pTex;

	D3DXVECTOR4 Light_Vec = (D3DXVECTOR4)lightVec;
	D3DXVECTOR4 Ink_Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	//�e���Z�b�g
	toon_->SetMatrix("World", &World);
	toon_->SetMatrix("WView", &camInfo.view);
	toon_->SetMatrix("WVProj", &wvp);
	toon_->SetVector("Light", &Light_Vec);
	toon_->SetVector("Ambient", &Ambient);
	toon_->SetTexture("ToonTex", toonTex);
	toon_->SetVector("InkColor", &Ink_Color);
	toon_->SetFloat("InkSize", 1.0f);
}
