#include "Bullet_Shotgun.h"
#include "../../../../Effekseer/Effekseer.h"
#include "../Weapon.h"
#include "../../Player.h"
#include "../../../../Camera/Camera.h"

Bullet_Shotgun::Bullet_Shotgun()
{
	for (int i = 0; i < BulletNum; i++)
	{
		bullet_[i] = new CEffekseer(CEffekseer::Effect_Bullet);
	}
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
		if (bullet_[i]->GetVisible())
		{
			bullet_[i]->Play();
			bullet_[i]->SetPosition(bullet_[i]->GetPosition() + bulletVector_[i] * 1.0f);
		}
		if (shot_)
		{
			count_++;
			if (count_  > 3 * 60)
			{
				count_ = 0;
				shot_ = false;
				bullet_[i]->SetVisible(false);
			}
		}

		bullet_[i]->Update();
	}
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
