#include "Enemy_HigeUI.h"
#include "../Billboard/Billboard.h"
#include "Enemy_Hige.h"

EnemyHigeUI::EnemyHigeUI(EnemyHige* enemy)
{
	if (!enemy) return;
	hige_ = enemy;
	lifeBar_ = Object::Create<Billboard>();
	lifeBar_->SetTexture(TextureManager::Tex_SliderBar);
}

EnemyHigeUI::~EnemyHigeUI()
{
}

void EnemyHigeUI::Init()
{
	if (!hige_)return;
	lifeBar_->SetScale(2.0f,1.0f, 2.0f);
	lifeBar_->SetPosition(hige_->GetPosition().x, hige_->GetPosition().y + 2.0f, hige_->GetPosition().z);
	lifeBar_->SetColor(D3DCOLOR_RGBA(255,100,100,255));
}

void EnemyHigeUI::Uninit()
{
}

void EnemyHigeUI::Update()
{
	if (!hige_)return;
	float rate = hige_->GetLife() / 100.0f;
	lifeBar_->SetScale(2.0f * rate, 0.5f, 2.0f * rate);
	lifeBar_->SetPosition(hige_->GetPosition().x, hige_->GetPosition().y + 2.0f, hige_->GetPosition().z);
}

void EnemyHigeUI::Draw()
{
}
