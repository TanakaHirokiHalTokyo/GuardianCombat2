#include "Bullet_Shotgun.h"
#include "../../../../Effekseer/Effekseer.h"
#include "../Weapon.h"
#include "../../Player.h"
#include "../../../../Camera/Camera.h"
#include "../../../../Debug/Debug.h"

Bullet_Shotgun::Bullet_Shotgun()
{
	for (int i = 0; i < BulletNum; i++)
	{
		bullet_[i] = new CEffekseer(CEffekseer::Effect_Bullet);
	}
	debug_ = Object::Create<DebugSphere>();
}

Bullet_Shotgun::~Bullet_Shotgun()
{
	for (int i = 0; i < BulletNum; i++)
	{
		bullet_[i]->Uninit();
		delete bullet_[i];
		bullet_[i] = nullptr;
	}
}

void Bullet_Shotgun::Init()
{
	for (int i = 0; i < BulletNum; i++)
	{
		shot_ = false;
		bullet_[i]->Init();
		bullet_[i]->SetScale(0.3f, 0.3f, 0.3f);
		bullet_[i]->SetVisible(false);
	}
	debug_->SetRadius(0.2f);
}

void Bullet_Shotgun::Uninit()
{
	for (int i = 0; i < BulletNum; i++)
	{
		bullet_[i]->Uninit();
	}
}

void Bullet_Shotgun::Update()
{
	for (int i = 0; i < BulletNum; i++)
	{
		if (shot_)
		{
			count_++;
			bullet_[i]->Play();
			if (count_  > 3 * 60)
			{
				count_ = 0;
				shot_ = false;
				bullet_[i]->SetVisible(false);
			}
		}
		if (bullet_[i]->GetVisible())
		{
			bullet_[i]->SetPosition(bullet_[i]->GetPosition() + bulletVector_[i] * 1.0f);
		}

		bullet_[i]->Update();
	}
	debug_->SetPosition(bullet_[0]->GetPosition());
	debug_->SetPositionY(bullet_[0]->GetPosition().y + (debug_->GetRadius() / 2.0f));
}

void Bullet_Shotgun::BeginDraw()
{
}

void Bullet_Shotgun::Draw()
{
	for (int i = 0; i < BulletNum; i++)
	{
		if (bullet_[i]->GetVisible())
		{
			bullet_[i]->Draw();
		}
	}
}

void Bullet_Shotgun::EndDraw()
{
}

void Bullet_Shotgun::SetShooting()
{
	if (!shot_)
	{
		shot_ = true;
		for (int i = 0; i < BulletNum; i++)
		{
			bullet_[i]->SetPosition(weapon_->GetPosition());
			bulletVector_[i] = weapon_->GetPlayer()->GetCamera()->GetFront();
			bullet_[i]->SetVisible(true);
		}
	}
}

bool Bullet_Shotgun::GetShooting()
{
	return shot_;
}
