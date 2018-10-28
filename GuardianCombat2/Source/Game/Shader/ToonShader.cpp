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

	//Camera 情報
	CAMERA_INFO& camInfo = Camera::GetCameraInfo();

	D3DXMATRIX& World = world;	//ワールド座標
	D3DXMATRIX wvp = world * camInfo.view * camInfo.proj;	//ワールドビュープロジェクション
	DirectionalLight* light = GameManager::GetDirectionalLight();
	D3DXVECTOR3 lightVec = light->GetVector();
	D3DXVECTOR4 Ambient = light->GetAmbient();
	LPDIRECT3DTEXTURE9 toonTex = TextureManager::GetTexture(TextureManager::Tex_Toon).pTex;

	//各情報セット
	toon_->SetMatrix("World", &world);
	toon_->SetMatrix("WView", &camInfo.view);
	toon_->SetMatrix("WVProj", &wvp);
	toon_->SetVector("Light", &(D3DXVECTOR4)lightVec);
	toon_->SetVector("Ambient", &Ambient);
	toon_->SetTexture("ToonTex", toonTex);
	toon_->SetVector("InkColor", &D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
	toon_->SetFloat("InkSize", 1.0f);
}
