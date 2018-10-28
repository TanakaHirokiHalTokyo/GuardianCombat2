#include "ShadowVolume.h"
#include "ShaderManager.h"
#include "../../Camera/Camera.h"
#include "../../Light/Light.h"
#include "../GameManager/GameManager.h"
#include "../../Texture/Texture.h"

LPD3DXEFFECT ShadowVolume::effect_ = NULL;

LPD3DXEFFECT ShadowVolume::GetEffect()
{
	if (effect_ == NULL)
	{
		effect_ = ShaderManager::GetEffect(ShaderManager::Shader_ShadowVolume);
	}
	return effect_;
}

void ShadowVolume::SetInfo(D3DXMATRIX world)
{
	if (effect_ == NULL)
	{
		effect_ = ShaderManager::GetEffect(ShaderManager::Shader_ShadowVolume);
	}
	CAMERA_INFO camInfo = Camera::GetCameraInfo();
	D3DXMATRIX wvp = world * camInfo.view * camInfo.proj;	//ワールドビュープロジェクション
	DirectionalLight* light = GameManager::GetDirectionalLight();
	D3DXVECTOR3 lightVec = light->GetVector();
	D3DXVECTOR4 Ambient = light->GetAmbient();


	effect_->SetMatrix("World",&world);
	effect_->SetMatrix("WView",&camInfo.view);
	effect_->SetMatrix("WVProj",&wvp);
	effect_->SetMatrix("Proj",&camInfo.proj);
	effect_->SetVector("Light",&(D3DXVECTOR4)lightVec);
	effect_->SetVector("Ambient",&Ambient);
	effect_->SetVector("VolumeColor",&D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f));
	effect_->SetFloat("NearZ",1);
	effect_->SetFloat("FarZ",1000);
}
