
#include "../../main.h"
#include "GameManager.h"
#include "../../Scene/Scene.h"
#include "../../Texture/Texture.h"
#include "../../Scene/Fade.h"
#include "../../Scene/TitleScene.h"
#include "../../DInput/DirectInput.h"
#include "../../Sound/Sound.h"
#include "../../Imgui/ImguiManager.h"
#include "../../Scene/GameScene.h"
#include "../Shader/ShaderManager.h"
#include "../../DirectXRenderer.h"	
#include "../../Light/Light.h"

Scene* GameManager::scene_ = NULL;
Fade* GameManager::fade_ = NULL;
DirectionalLight* GameManager::light_ = NULL;
std::string GameManager::sceneTag_ = "None";
bool GameManager::gameObjectLoaded_ = false;
Player* GameManager::player_ = nullptr;

void GameManager::Init()
{
	TextureManager::LoadAll();
	SoundManager::Init();
	ShaderManager::LoadAll();
	/*fade_ = new Fade();
	fade_->Init();
	fade_->FadeIn();*/
	SetScene(new TitleScene());
}
void GameManager::Uninit()
{
	if (scene_ != NULL)
	{
		SetScene(NULL);
	}
	if (fade_ != NULL)
	{
		fade_->Uninit();
		SAFE_DELETE(fade_);
	}
	Object::GameObjectReleaseAll();			//ゲームオブジェクトすべて解放。
	SoundManager::Uninit();					//サウンド終了処理
	TextureManager::ReleaseAll();			//テクスチャ解放
	ShaderManager::ReleaseAll();			//シェーダー解放
}
void GameManager::Update()
{
	if (scene_ != NULL)
	{
		scene_->Update();
		//fade_->Update();
	}
	if (GetKeyboardTrigger(DIK_F9))
	{
		if (GetMouseCursorShow())
		{
			SetMouseCursorShow(false);
		}
		else
		{
			SetMouseCursorShow(true);
		}
	}
}
void GameManager::Draw()
{
	if (scene_ != NULL)
	{
		scene_->BeginDraw();
		CRendererDirectX::ResetRenderTarget();
		scene_->Draw();
		scene_->EndDraw();
	}
	//fade_->Draw();
}

void GameManager::SetScene(Scene * scene)
{
	if (scene_ != NULL)
	{
		scene_->Uninit();
		delete scene_;
		scene_ = NULL;	
	}

	scene_ = scene;

	if (scene_ != NULL)
	{
		scene_->Init();
	}
}

Scene * GameManager::GetScene()
{
	return scene_;
}

Fade * GameManager::GetFade()
{
	fade_->Init();
	return fade_;
}

void GameManager::SetDirectionalLight(DirectionalLight * light)
{
	light_ = light;
}

DirectionalLight * GameManager::GetDirectionalLight()
{
	return light_;
}
void GameManager::SetSceneTag(std::string sceneTag)
{
	sceneTag_ = sceneTag;
}

std::string GameManager::GetSceneTag()
{
	return sceneTag_;
}

void GameManager::SetGameObjectLoad(bool loaded)
{
	gameObjectLoaded_ = loaded;
}

bool GameManager::GetGameObjectLoad()
{
	return gameObjectLoaded_;
}

Player* GameManager::SetPlayer(Player * player)
{
	player_ = player;

	return player_;
}

Player * GameManager::GetPlayer()
{
	return player_;
}
