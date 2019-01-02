#include "Object.h"
#include "../main.h"
#include "../Collision/Collision.h"
#include "../Imgui/ImguiManager.h"
#include "Enemy\Enemy_Hige.h"
#include "Player\Player.h"
#include "Player\Weapon\Bullet\Bullet.h"
#include "Enemy\Enemy_HigeAvater.h"
#include "Cube\Cube.h"

#define DEBUG_PLAYER	(false)

vector<Object*> Object::objects_ = {};
vector<Sphere*> Object::enemyCollisions_ = {};
vector<Sphere*> Object::enemyAvaterCollisions_ = {};
vector<OBB*> Object::enemycubeCollisions_ = {};
vector<Sphere*> Object::playerCollision_ = {};
OBB* Object::enemyBurstCollision_ = nullptr;
vector<Sphere*> Object::playerBulletCollisions_ = {};

Object::Object()
	: transform_{ D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1),D3DXVECTOR3(0,0,0) }
	, exist_(true)
	, type_(ObjectType::obj_None)
{

}
void Object::Release()
{
	this->Uninit();
	delete this;
}
void Object::InitAll()
{
	for (unsigned int i = 0; i < objects_.size(); i++)
	{
		objects_[i]->Init();
	}
}
void Object::UpdateAll()
{
	for (size_t i = 0; i < objects_.size();)
	{
		if (objects_[i]->exist_ != true)
		{
			if (objects_[i] != nullptr)
			{
				objects_[i]->Uninit();
				SAFE_DELETE(objects_[i]);
				objects_.erase(objects_.begin() + i);
			}
		}
		else
		{
			if (!objects_[i]->pause_)
			{
				objects_[i]->Update();
			}
			++i;
		}
	}
}
void Object::BeginDrawAll()
{
	for (size_t i = 0; i < objects_.size(); i++)
	{
		if (objects_[i]->GetVisible())
		{
			auto obj = objects_[i];
			obj->BeginDraw();
		}
	}
}
void Object::DrawAll()
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		if (objects_[i]->GetVisible())
		{
			auto obj = objects_[i];

			obj->Draw();
		}
	}
}
void Object::EndDrawAll()
{
	for (size_t i = 0; i < objects_.size(); i++)
	{
		if (objects_[i]->GetVisible())
		{
			auto obj = objects_[i];

			obj->EndDraw();
		}
	}
}
void Object::ReleaseAll()
{
	for (size_t i = 0; i < objects_.size(); )
	{
		auto obj = objects_[i];
		obj->Uninit();
		SAFE_DELETE(obj);
		objects_.erase(objects_.begin() + i);
	}
}
void Object::JudgementAll()
{
	//無敵状態の場合当たり判定を行わない
	if (!DEBUG_PLAYER)
	{
		for (size_t i = 0; i < enemycubeCollisions_.size(); i++)
		{
			for (size_t j = 0; j < playerCollision_.size(); j++)
			{
				if (enemycubeCollisions_[i]->enable_)
				{
					//Playerとキューブの当たり判定
					if (isCollisionOBBtoSphere(*enemycubeCollisions_[i], *playerCollision_[j]))
					{
						//敵の弾の情報取得
						Cube* cube = (Cube*)enemycubeCollisions_[i]->object_;
						//プレイヤー情報取得
						Player* player = (Player*)playerCollision_[j]->object_;
						if (!cube->GetHit())
						{
							cube->Hit();
							if (!player->GetInvincible())
							{
								player->DecreaseLife(cube->GetAttackValue());
							}
						}

						ImGui::Begin("DEBUG COLLISION");
						ImGui::Text("Hit Cube!!!");
						ImGui::End();
					}
				}
			}
		}

		for (size_t i = 0; i < playerCollision_.size(); i++)
		{
			if (enemyBurstCollision_ && enemyBurstCollision_->enable_)
			{
				if (isCollisionOBBtoSphere(*enemyBurstCollision_, *playerCollision_[i]))
				{
					//敵の情報取得
					EnemyHige* enemy = (EnemyHige*)enemyBurstCollision_->object_;
					//プレイヤー情報取得
					Player* player = (Player*)playerCollision_[i]->object_;
					if (!player->GetInvincible())
					{
						player->DecreaseLife(enemy->GetBurstParameter().dps / (float)GameFPS);
					}
					ImGui::Begin("DEBUG COLLISION");
					ImGui::Text("Hit Burst!!!");
					ImGui::End();
				}
			}
		}
		for (size_t i = 0; i < playerBulletCollisions_.size(); i++)
		{
			for (size_t j = 0; j < enemyCollisions_.size(); j++)
			{
				Bullet* bullet = (Bullet*)playerBulletCollisions_[i]->object_;
				Enemy* enemy = (Enemy*)enemyCollisions_[j]->object_;

				if (bullet->GetShooting())
				{
					if (isCollisionSphere(*playerBulletCollisions_[i],*enemyCollisions_[j]))
					{
						if (!enemy->GetInvincible())
						{
							enemy->DecreaseLife(0.3f);
						}
						bullet->Destroy();

						ImGui::Begin("DEBUG COLLISION");
						ImGui::Text("Hit PlayerBullet!!!");
						ImGui::End();
					}	
				}
			}
			for (size_t j = 0; j < enemyAvaterCollisions_.size(); j++)
			{
				Bullet* bullet = (Bullet*)playerBulletCollisions_[i]->object_;
				EnemyHige_Avater* enemy = (EnemyHige_Avater*)enemyAvaterCollisions_[j]->object_;

				if (bullet->GetShooting())
				{
					if (enemy->GetVisible())
					{
						if (isCollisionSphere(*playerBulletCollisions_[i], *enemyAvaterCollisions_[j]))
						{
							enemy->Destroy();
							bullet->Destroy();
							ImGui::Begin("DEBUG COLLISION");
							ImGui::Text("Hit PlayerBullet!!!");
							ImGui::End();
						}
					}
				}
			}
		}
		for (size_t i = 0; i < enemyCollisions_.size(); i++)
		{
			Enemy* enemy = (Enemy*)enemyCollisions_[i]->object_;
			if (!enemy->GetCollisionEnable()) continue;

			for (size_t j = 0; j < playerCollision_.size(); j++)
			{
				Player* player = (Player*)playerCollision_[j]->object_;
				if (isCollisionSphere(*enemyCollisions_[i], *playerCollision_[j]))
				{
					if (!player->GetInvincible())
					{
						player->DecreaseLife(enemy->GetCollisionDPS());
					}

					ImGui::Begin("DEBUG COLLISION");
					ImGui::Text("Hit Rush!!!");
					ImGui::End();
				}
			}
		}
	}
	
}
void Object::CollisionReleaseAll()
{
	for (size_t i = 0; i < enemycubeCollisions_.size();)
	{
		auto collision = enemycubeCollisions_[i];
		if(collision) delete collision;
		enemycubeCollisions_.erase(enemycubeCollisions_.begin() + i);
	}
	for (size_t i = 0; i < playerCollision_.size();)
	{
		auto collision = playerCollision_[i];
		if(collision) delete collision;
		playerCollision_.erase(playerCollision_.begin() + i);
	}
	for (size_t i = 0; i < enemyCollisions_.size();)
	{
		auto collision = enemyCollisions_[i];
		if (collision) delete collision;
		enemyCollisions_.erase(enemyCollisions_.begin() + i);
	}
	for (size_t i = 0; i < playerBulletCollisions_.size();)
	{
		auto collision = playerBulletCollisions_[i];
		if (collision) delete collision;
		playerBulletCollisions_.erase(playerBulletCollisions_.begin() + i);
	}
	for (size_t i = 0; i < enemyAvaterCollisions_.size();)
	{
		auto collision = enemyAvaterCollisions_[i];
		if (collision)delete collision;
		enemyAvaterCollisions_.erase(enemyAvaterCollisions_.begin() + i);
	}

	if (enemyBurstCollision_)
	{
		delete enemyBurstCollision_;
		enemyBurstCollision_ = nullptr;
	}
}

void Object::SetPauseAll(bool flag)
{
	for (unsigned int i = 0; i < objects_.size(); ++i)
	{
		objects_[i]->pause_ = flag;
	}
}

void Object::SetUseShader(bool flag)
{
	useShader_ = flag;
}

bool Object::GetUseShader()
{
	return useShader_;
}

void Object::SetWorld(D3DXMATRIX * world)
{
	world_ = *world;
}

D3DXMATRIX * Object::GetWorld()
{
	return &world_;
}
