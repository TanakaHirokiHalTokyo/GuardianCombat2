
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
#include "../Blur/Blur.h"

Scene* GameManager::scene_ = NULL;
Fade* GameManager::fade_ = NULL;
DirectionalLight* GameManager::light_ = NULL;
std::string GameManager::sceneTag_ = "None";
Player* GameManager::player_ = nullptr;
Blur* GameManager::blur_ = nullptr;
bool GameManager::playingGame_ = false;
bool GameManager::gameOver_ = false;
bool GameManager::gameClear_ = false;
bool GameManager::enableEdit_ = true;
bool GameManager::ending_ = false;
int GameManager::playerSkill1_ = -1;
int GameManager::playerSkill2_ = -1;
bool GameManager::returnTitle_ = false;

void GameManager::Init()
{
	TextureManager::LoadAll();
	SoundManager::Init();
	ShaderManager::LoadAll();
	fade_ = new Fade();
	fade_->Init();
	fade_->FadeIn();
	SetScene(new TitleScene());

	blur_ = new Blur();
}
void GameManager::Uninit()
{
	if (scene_ != NULL)
	{
		SetScene(NULL);
	}
	if (fade_ != nullptr)
	{
		fade_->Uninit();
		SAFE_DELETE(fade_);
	}
	if (blur_)
	{
		delete blur_;
		blur_ = nullptr;
	}
	Object::ReleaseAll();							//オブジェクト解放
	Object::CollisionReleaseAll();				//コリジョン解放
	SoundManager::Uninit();						//サウンド終了処理
	TextureManager::ReleaseAll();			//テクスチャ解放
	ShaderManager::ReleaseAll();			//シェーダー解放
}
void GameManager::Update()
{
	if (scene_ != nullptr)
	{
		scene_->Update();
		fade_->Update();
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
	
	if (GetKeyboardTrigger(DIK_F1))
	{
		enableEdit_ = !enableEdit_;
	}
	if (playingGame_)	enableEdit_ = false;
}
void GameManager::Draw()
{
	if (scene_ != nullptr)
	{
		scene_->BeginDraw();
		CRendererDirectX::ResetRenderTarget();
		scene_->Draw();
		scene_->EndDraw();
		fade_->Draw();
	}
}

void GameManager::SetScene(Scene * scene)
{
	if (scene_ != nullptr)
	{
		scene_->Uninit();
		delete scene_;
		scene_ = nullptr;	
	}

	scene_ = scene;

	if (scene_ != nullptr)
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

void GameManager::GameOver(bool flag)
{
	gameOver_ = flag;
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

Blur * GameManager::GetBlur()
{
	return blur_;
}