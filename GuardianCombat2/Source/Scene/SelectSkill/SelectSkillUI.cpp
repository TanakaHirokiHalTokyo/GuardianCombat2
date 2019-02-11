#include "SelectSkillUI.h"
#include "../../Texture/Texture.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"
#include "../../main.h"
#include "../SelectSkillScene.h"
#include <d3dx9.h>
#include <d3d9.h>

constexpr float ICON_SIZE = 150.0f;		//アイコンの大きさ
constexpr float EXP_HEIGHT = (float)ScreenHeight / 1.5f;		//説明文縦幅
constexpr float ICON_OFFSET = (float)((float)ScreenWidth - ICON_SIZE * SelectSkillScene::SKILL_MAX) / 2.0f;		//アイコン座標相対座標
constexpr int EXP_ALPHA = 120;		//説明文の透明度
constexpr int ICON_ALPHA = 50;		//アイコンの非選択時の透明度

SelectSkillUI::SelectSkillUI(SelectSkillScene*scene)
{
	scene_ = scene;

	//back_ = Object::Create<Texture>(TextureManager::Tex_Fade);
	iconDash_ = Object::Create<Texture>(TextureManager::Tex_IconDash);
	iconHeal_ = Object::Create<Texture>(TextureManager::Tex_IconHeal);
	iconThunder_ = Object::Create<Texture>(TextureManager::Tex_IconThunder);
	iconSpeedUp_ = Object::Create<Texture>(TextureManager::Tex_IconSpeedUp);
	expDash_ = Object::Create<Texture>(TextureManager::Tex_ExpDash);
	expHeal_ = Object::Create<Texture>(TextureManager::Tex_ExpHeal);
	expThunder_ = Object::Create<Texture>(TextureManager::Tex_ExpThunder);
	expSpeedUp_ = Object::Create<Texture>(TextureManager::Tex_ExpSpeedUp);
}

SelectSkillUI::~SelectSkillUI()
{

}

void SelectSkillUI::Init()
{
	iconDash_->SetDrawSize(ICON_SIZE,ICON_SIZE);
	iconHeal_->SetDrawSize(ICON_SIZE, ICON_SIZE);
	iconThunder_->SetDrawSize(ICON_SIZE, ICON_SIZE);
	iconSpeedUp_->SetDrawSize(ICON_SIZE, ICON_SIZE);

	expDash_->SetDrawSize((float)ScreenWidth / 1.5f,EXP_HEIGHT / 1.5f);
	expHeal_->SetDrawSize((float)ScreenWidth / 1.5f, EXP_HEIGHT / 1.5f);
	expThunder_->SetDrawSize((float)ScreenWidth / 1.5f, EXP_HEIGHT / 1.5f);
	expSpeedUp_->SetDrawSize((float)ScreenWidth / 1.5f, EXP_HEIGHT / 1.5f);
	//back_->SetDrawSize((float)ScreenWidth, EXP_HEIGHT);

	expDash_->SetColor(D3DCOLOR_RGBA(255,255,255, 255));
	expHeal_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	expThunder_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	expSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	//back_->SetColor(D3DCOLOR_RGBA(255, 255, 255, EXP_ALPHA));

	iconDash_->SetColor(D3DCOLOR_RGBA(255, 255, 255, ICON_ALPHA));
	iconHeal_->SetColor(D3DCOLOR_RGBA(255, 255, 255, ICON_ALPHA));
	iconThunder_->SetColor(D3DCOLOR_RGBA(255, 255, 255, ICON_ALPHA));
	iconSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 255, 255, ICON_ALPHA));

	iconDash_->SetPosition((float)ICON_SIZE * SelectSkillScene::SKILL_DASH + ICON_OFFSET,(float)ScreenHeight - ICON_SIZE - ICON_SIZE / 10.0f,0.0f);
	iconHeal_->SetPosition((float)ICON_SIZE * SelectSkillScene::SKILL_HEAL + ICON_OFFSET, (float)ScreenHeight - ICON_SIZE - ICON_SIZE / 10.0f, 0.0f);
	iconThunder_->SetPosition((float)ICON_SIZE * SelectSkillScene::SKILL_THUDER + ICON_OFFSET, (float)ScreenHeight - ICON_SIZE - ICON_SIZE / 10.0f, 0.0f);
	iconSpeedUp_->SetPosition((float)ICON_SIZE * SelectSkillScene::SKILL_SPEEDUP + ICON_OFFSET, (float)ScreenHeight - ICON_SIZE - ICON_SIZE / 10.0f, 0.0f);

	expDash_->SetVisible(false);
	expHeal_->SetVisible(false);
	expThunder_->SetVisible(false);
	expSpeedUp_->SetVisible(false);

	switch (scene_->GetSelectingNumber())
	{
	case SelectSkillScene::SKILL_DASH:
		expDash_->SetVisible(true);
		iconDash_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		break;
	case SelectSkillScene::SKILL_HEAL:
		expHeal_->SetVisible(true);
		iconHeal_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		break;
	case SelectSkillScene::SKILL_THUDER:
		expThunder_->SetVisible(true);
		iconThunder_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		break;
	case SelectSkillScene::SKILL_SPEEDUP:
		expSpeedUp_->SetVisible(true);
		iconSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		break;
	default:
		break;
	}
}

void SelectSkillUI::Uninit()
{
}

void SelectSkillUI::Update()
{
	//選択済みのスキル表示変更
	switch (scene_->GetSelectedSkill1())
	{
	case SelectSkillScene::SKILL_DASH:
		iconDash_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	case SelectSkillScene::SKILL_HEAL:
		iconHeal_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	case SelectSkillScene::SKILL_THUDER:
		iconThunder_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	case SelectSkillScene::SKILL_SPEEDUP:
		iconSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	default:
		break;
	}
	switch (scene_->GetSelectedSkill2())
	{
	case SelectSkillScene::SKILL_DASH:
		iconDash_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	case SelectSkillScene::SKILL_HEAL:
		iconHeal_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	case SelectSkillScene::SKILL_THUDER:
		iconThunder_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	case SelectSkillScene::SKILL_SPEEDUP:
		iconSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		break;
	default:
		break;
	}
	if (scene_->GetSelectingOldNumber() != scene_->GetSelectingNumber())
	{

		//前フレーム表示していた説明非表示
		switch (scene_->GetSelectingOldNumber())
		{
		case SelectSkillScene::SKILL_DASH:
			iconDash_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 50));
			expDash_->SetVisible(false);
			break;
		case SelectSkillScene::SKILL_HEAL:
			iconHeal_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 50));
			expHeal_->SetVisible(false);
			break;
		case SelectSkillScene::SKILL_THUDER:
			iconThunder_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 50));
			expThunder_->SetVisible(false);
			break;
		case SelectSkillScene::SKILL_SPEEDUP:
			iconSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 50));
			expSpeedUp_->SetVisible(false);
			break;
		default:
			break;
		}
		//表示する説明更新
		switch (scene_->GetSelectingNumber())
		{
		case SelectSkillScene::SKILL_DASH:
			iconDash_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			expDash_->SetVisible(true);
			break;
		case SelectSkillScene::SKILL_HEAL:
			iconHeal_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			expHeal_->SetVisible(true);
			break;
		case SelectSkillScene::SKILL_THUDER:
			iconThunder_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			expThunder_->SetVisible(true);
			break;
		case SelectSkillScene::SKILL_SPEEDUP:
			iconSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			expSpeedUp_->SetVisible(true);
			break;
		default:
			break;
		}
	}
}

void SelectSkillUI::Draw()
{
}

void SelectSkillUI::ResetIconColor(int skill_index)
{
	switch (skill_index)
	{
	case (int)SelectSkillScene::SKILL_DASH:
		iconDash_->SetColor(D3DCOLOR_RGBA(255,100,100,50));
		break;
	case (int)SelectSkillScene::SKILL_HEAL:
		iconHeal_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 50));
		break;
	case (int)SelectSkillScene::SKILL_THUDER:
		iconThunder_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 50));
		break;
	case (int)SelectSkillScene::SKILL_SPEEDUP:
		iconSpeedUp_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 50));
		break;
	default:
		break;
	}
}
