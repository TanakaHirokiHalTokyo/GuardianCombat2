#include "Weapon.h"
#include "Bullet\Bullet.h"
#include "../../GameManager/GameManager.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Camera/Camera.h"
#include "../Player.h"
#include "../../XModel/XModel.h"
void Weapon::ShotBullet(int BulletNum)
{
	
	D3DXVECTOR3 vec = GetPosition();
	D3DXVec3Normalize(&vec, &vec);
	for (size_t i = 0; i < BulletNum; i++)
	{
		if (!bullet_[i].GetShooting())
		{
			Player* player = GameManager::GetPlayer();
			bullet_[i].SetShooting(model_->GetPosition(),player->GetCamera()->GetFront(),0.7f );
			break;
		}
	}
}
