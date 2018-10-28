#include "Portal.h"
#include "../../Effekseer/Effekseer.h"

Portal::Portal()
{
	portal_ = new CEffekseer(CEffekseer::Effect_Portal);
	portal_->Init();
	portal_->RepeatEffect(true);
	portal_->SetVisible(true);
}

Portal::~Portal()
{
	if (portal_ != nullptr)
	{
		portal_->Uninit();
		delete portal_;
		portal_ = nullptr;
	}
}

void Portal::Init()
{
	SetPosition(0, 0, 0);
	SetScale(1.0f, 1.0f, 1.0f);

	portal_->SetPosition(0,0,0);
	portal_->SetScale(1.0f, 1.0f, 1.0f);
}

void Portal::Uninit()
{
	portal_->Uninit();
}

void Portal::Update()
{
	portal_->SetPosition(GetPosition());
	portal_->SetScale(GetScale());
	portal_->SetRotate(GetRotate());
	portal_->Update();
}

void Portal::BeginDraw()
{
}

void Portal::Draw()
{
	portal_->Draw();
}

void Portal::EndDraw()
{
}
