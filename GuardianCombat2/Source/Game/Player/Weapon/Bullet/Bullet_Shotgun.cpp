#include "Bullet_Shotgun.h"
#include "../../../../Effekseer/Effekseer.h"

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
		bullet_[i]->Init();
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
		bullet_[i]->Draw();
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
	}
}

bool Bullet_Shotgun::GetShooting()
{
	return shot_;
}
