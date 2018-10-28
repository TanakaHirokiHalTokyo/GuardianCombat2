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

	//Camera 情報
	CAMERA_INFO& camInfo = Camera::GetCameraInfo();

	D3DXMATRIX& World = world;	//ワールド座標
	D3DXMATRIX wvp = world * camInfo.view * camInfo.proj;	//ワールドビュープロジェクション

	effect_->SetMatrix("mWVP",&wvp);
}
