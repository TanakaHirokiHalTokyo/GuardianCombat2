#include "SelectSkillUI.h"
#include "../../Texture/Texture.h"

SelectSkillUI::SelectSkillUI()
{
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
}

void SelectSkillUI::Uninit()
{
}

void SelectSkillUI::Update()
{
}

void SelectSkillUI::Draw()
{
}
