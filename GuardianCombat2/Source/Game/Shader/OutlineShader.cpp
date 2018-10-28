#include "OutlineShader.h"
#include "ShaderManager.h"
#include "../../Camera/Camera.h"

LPD3DXEFFECT OutlineShader::effect_ = NULL;


LPD3DXEFFECT OutlineShader::GetEffect()
{
	if (effect_ == NULL)
	{
		effect_ = ShaderManager::GetEffect(ShaderManager::Shader_Outline);
	}

	return effect_;
}

void OutlineShader::SetInfo(D3DXMATRIX world)
{
	if (effect_ == NULL)
	{
		effect_ = ShaderManager::GetEffect(ShaderManager::Shader_Outline);
	}

	//Camera ���
	CAMERA_INFO& camInfo = Camera::GetCameraInfo();

	D3DXMATRIX& World = world;	//���[���h���W
	D3DXMATRIX wvp = world * camInfo.view * camInfo.proj;	//���[���h�r���[�v���W�F�N�V����

	effect_->SetMatrix("mWVP",&wvp);
}
