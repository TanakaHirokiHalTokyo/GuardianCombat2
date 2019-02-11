#include "SelectSkillEffect.h"
#include "../SelectSkillScene.h"
#include "../../Effekseer/Effekseer.h"
#include "../../Camera/StayCamera.h"

SelectSkillEffect::SelectSkillEffect(SelectSkillScene* scene)
{
	scene_ = scene;
	camera_ = Object::Create<StayCamera>();
	efDash_	= new CEffekseer(CEffekseer::Effect_Dash);
	efHeal_	= new CEffekseer(CEffekseer::Effect_Heal);
	efThunder_	= new CEffekseer(CEffekseer::Effect_Thunder);
	efSpeedUp_	 = new CEffekseer(CEffekseer::Effect_SpeedUp);
}

SelectSkillEffect::~SelectSkillEffect()
{
	if (efDash_) {
		efDash_->Uninit();
		delete efDash_;
		efDash_ = nullptr;
	}
	if (efHeal_){
		efHeal_->Uninit();
		delete efHeal_;
		efHeal_ = nullptr;
	}
	if (efThunder_) {
		efThunder_->Uninit();
		delete efThunder_;
		efThunder_ = nullptr;
	}
	if (efSpeedUp_) {
		efSpeedUp_->Uninit();
		delete efSpeedUp_;
		efSpeedUp_ = nullptr;
	}
}

void SelectSkillEffect::Init()
{
	efDash_->Init();
	efHeal_->Init();
	efThunder_->Init();
	efSpeedUp_->Init();

	camera_->SetAt(0.0f, 0.0f, 0.0f);
	camera_->SetPosition(0.0f, 0.0f, 10.0f);

	efDash_->SetVisible(false);
	efHeal_->SetVisible(false);
	efThunder_->SetVisible(false);
	efSpeedUp_->SetVisible(false);

	efDash_->RepeatEffect(true);
	efHeal_->RepeatEffect(true);
	efThunder_->RepeatEffect(true);
	efSpeedUp_->RepeatEffect(true);

	switch (scene_->GetSelectingNumber())
	{
	case SelectSkillScene::SKILL_DASH:
		efDash_->SetVisible(true);
		break;
	case SelectSkillScene::SKILL_HEAL:
		efHeal_->SetVisible(true);
		break;
	case SelectSkillScene::SKILL_THUDER:
		efThunder_->SetVisible(true);
		break;
	case SelectSkillScene::SKILL_SPEEDUP:
		efSpeedUp_->SetVisible(true);
		break;
	default:
		break;
	}
}

void SelectSkillEffect::Uninit()
{
	efDash_->Uninit();
	efHeal_->Uninit();
	efThunder_->Uninit();
	efSpeedUp_->Uninit();
}

void SelectSkillEffect::Update()
{
	if (scene_->GetSelectingOldNumber() != scene_->GetSelectingNumber())
	{
		switch (scene_->GetSelectingOldNumber())
		{
		case SelectSkillScene::SKILL_DASH:
			efDash_->SetVisible(false);
			break;
		case SelectSkillScene::SKILL_HEAL:
			efHeal_->SetVisible(false);
			break;
		case SelectSkillScene::SKILL_THUDER:
			efThunder_->SetVisible(false);
			break;
		case SelectSkillScene::SKILL_SPEEDUP:
			efSpeedUp_->SetVisible(false);
			break;
		default:
			break;
		}
		
		switch (scene_->GetSelectingNumber())
		{
		case SelectSkillScene::SKILL_DASH:
			efDash_->SetVisible(true);
			break;
		case SelectSkillScene::SKILL_HEAL:
			efHeal_->SetVisible(true);
			break;
		case SelectSkillScene::SKILL_THUDER:
			efThunder_->SetVisible(true);
			break;
		case SelectSkillScene::SKILL_SPEEDUP:
			efSpeedUp_->SetVisible(true);
			break;
		default:
			break;
		}
	}
	efDash_->Update();
	efHeal_->Update();
	efThunder_->Update();
	efSpeedUp_->Update();
}


void SelectSkillEffect::Draw()
{
	efDash_->Draw();
	efHeal_->Draw();
	efThunder_->Draw();
	efSpeedUp_->Draw();
}

