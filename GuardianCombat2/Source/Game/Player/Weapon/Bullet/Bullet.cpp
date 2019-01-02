#include "Bullet.h"
#include "../../../Billboard/Billboard.h"

void Bullet::SetShooting(D3DXVECTOR3 pos, D3DXVECTOR3 vec, float speed)
{
	SetPosition(pos);
	bulletInfo_.bullet_->SetVisible(true);
	bulletInfo_.vector_ = vec;
	bulletInfo_.speed_ = speed;
	bulletInfo_.use_ = true;
}

void Bullet::Destroy()
{
	bulletInfo_.use_ = false;
	bulletInfo_.bullet_->SetVisible(false);
}
