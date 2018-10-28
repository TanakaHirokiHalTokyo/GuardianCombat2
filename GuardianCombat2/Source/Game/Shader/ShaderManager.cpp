#include <d3d9.h>
#include <d3dx9.h>
#include "ShaderManager.h"
#include "../../DirectXRenderer.h"

const ShaderManager::FxFile ShaderManager::shaderInfo_[ShaderManager::Shader_Max] = {
	"resource/Shader/Toon.fx",
	"resource/Shader/Outline.fx",
	"resource/Shader/ShadowVolume.fx",
	"resource/Shader/ShadowMap.fx",
	"resource/Shader/VertexBlend.fx",
};
LPD3DXEFFECT ShaderManager::effect_[ShaderManager::Shader_Max] = {};

void ShaderManager::LoadAll()
{
	for (int i = 0; i < ShaderManager::Shader_Max; ++i)
	{
		effect_[i] = nullptr;

		if (FAILED(D3DXCreateEffectFromFileA(CRendererDirectX::GetDevice(), shaderInfo_[i].fileName, NULL, NULL, 0, NULL, &effect_[i], NULL)))
		{
			MessageBoxA(NULL, "シェーダー読み込み失敗", "Error", MB_YESNO);
		}
	}
}
void ShaderManager::ReleaseAll()
{
	for (int i = 0; i < ShaderManager::Shader_Max; ++i)
	{
		if (effect_[i] != nullptr)
		{
			effect_[i]->Release();
			effect_[i] = nullptr;
		}
	}
}
LPD3DXEFFECT ShaderManager::GetEffect(ShaderManager::ShaderList type)
{
	return effect_[type];
}

