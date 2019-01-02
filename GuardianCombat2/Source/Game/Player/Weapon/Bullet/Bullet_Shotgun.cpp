#include "Bullet_Shotgun.h"
#include "../Weapon.h"
#include "../../Player.h"
#include "../../../../Camera/Camera.h"
#include "../../../../Debug/Debug.h"
#include "../../../Billboard/Billboard.h"
#include "../../../../Collision/Collision.h"

Bullet_Shotgun::Bullet_Shotgun()
{
	bulletInfo_.bullet_ = Object::Create<Billboard>();
	bulletInfo_.bullet_->SetTexture(TextureManager::Tex_SimpleBullet);

	bulletInfo_.collision_ = new Sphere();
	bulletInfo_.collision_->object_ = this;
	playerBulletCollisions_.emplace_back(bulletInfo_.collision_);

	bulletInfo_.debug_ = new DebugSphere();
	bulletInfo_.debug_->Init();
}

Bullet_Shotgun::~Bullet_Shotgun()
{
	if (bulletInfo_.debug_)
	{
		bulletInfo_.debug_->Uninit();
		delete bulletInfo_.debug_;
		bulletInfo_.debug_ = nullptr;
	}
}

void Bullet_Shotgun::Init()
{
	SetPosition(0,0,0);
	SetScale(0.02f,0.02f,0.02f);
	bulletInfo_.bullet_->SetVisible(false);
	bulletInfo_.bullet_->SetUseShader(false);
	bulletInfo_.bullet_->SetScale(0.02f,0.02f,0.02f);
	bulletInfo_.collision_->rad = bulletInfo_.bullet_->GetScale().x;
	bulletInfo_.count_ = 0;
	bulletInfo_.debug_->Init();
	bulletInfo_.use_ = false;
}

void Bullet_Shotgun::Uninit()
{
	
}

void Bullet_Shotgun::Update()
{
	if (bulletInfo_.use_)
	{
		bulletInfo_.count_++;
		if (bulletInfo_.count_ >= BulletAliveCount)
		{
			bulletInfo_.count_ = 0;
			bulletInfo_.use_ = false;
			bulletInfo_.bullet_->SetVisible(false);
			return;
		}

		SetPosition(GetPosition() + bulletInfo_.vector_ * bulletInfo_.speed_);
		
		bulletInfo_.bullet_->SetPosition(GetPosition());
		bulletInfo_.bullet_->SetScale(GetScale());

		bulletInfo_.collision_->pos = GetPosition();
	}
}

void Bullet_Shotgun::BeginDraw()
{
}

void Bullet_Shotgun::Draw()
{
	
}

void Bullet_Shotgun::EndDraw()
{
}

