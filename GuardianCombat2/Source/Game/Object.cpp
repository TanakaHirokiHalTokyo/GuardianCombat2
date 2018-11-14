#include "Object.h"
#include "../main.h"
#include "../Collision/Collision.h"
#include "../Imgui/ImguiManager.h"
#define DEBUG_PLAYER	(false)


vector<Object*> Object::objects_;
vector<Object*> Object::gameObjects_;
vector<OBB*> Object::enemyHormingCollisions_;
vector<Sphere*> Object::playerCollision_;

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
	//現在のシーンがゲームシーンかどうかを判断
	if (GameManager::GetSceneTag() == "GameScene")
	{
		for (unsigned int i = 0; i < gameObjects_.size(); i++)
		{
			gameObjects_[i]->exist_ = true;
			gameObjects_[i]->Init();
		}
	}
	else
	{
		for (unsigned int i = 0; i < objects_.size(); i++)
		{
			objects_[i]->Init();
		}
	}
}
void Object::UpdateAll()
{
	//現在のシーンがゲームシーンかどうかを判断
	if (GameManager::GetSceneTag() == "GameScene")
	{
		for (unsigned int i = 0; i < gameObjects_.size();)
		{
			if (gameObjects_[i]->exist_ != true)
			{
				if (gameObjects_[i] != nullptr)
				{
					gameObjects_[i]->Uninit();
					gameObjects_[i]->SetVisible(false);
				}
			}
			else
			{
				if (!gameObjects_[i]->pause_)
				{
					gameObjects_[i]->Update();
				}
				++i;
			}
		}
	}
	else
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
}
void Object::BeginDrawAll()
{
	//現在のシーンがゲームシーンかどうかを判断
	if (GameManager::GetSceneTag() == "GameScene")
	{
		for (size_t i = 0; i < gameObjects_.size(); i++)
		{
			if (gameObjects_[i]->GetVisible())
			{
				auto obj = gameObjects_[i];
				obj->BeginDraw();
			}
		}
	}
	else
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
}
void Object::DrawAll()
{
	//現在のシーンがゲームシーンかどうかを判断
	if (GameManager::GetSceneTag() == "GameScene")
	{
		for (size_t i = 0; i < gameObjects_.size(); i++)
		{
			if (gameObjects_[i]->GetVisible())
			{
				auto obj = gameObjects_[i];

				obj->Draw();
			}
		}
	}
	else
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
}
void Object::EndDrawAll()
{
	//現在のシーンがゲームシーンかどうかを判断
	if (GameManager::GetSceneTag() == "GameScene")
	{
		for (size_t i = 0; i < gameObjects_.size(); i++)
		{
			if (gameObjects_[i]->GetVisible())
			{
				auto obj = gameObjects_[i];

				obj->EndDraw();
			}
		}
	}
	else
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
void Object::GameObjectReleaseAll()
{
	for (unsigned int i = 0; i < gameObjects_.size();)
	{
		auto obj = gameObjects_[i];
		obj->Uninit();
		SAFE_DELETE(obj);
		gameObjects_.erase(gameObjects_.begin() + i);
	}
}
void Object::JudgementAll()
{
	for (size_t i = 0; i < enemyHormingCollisions_.size(); i++)
	{
		for (size_t j = 0; j < playerCollision_.size(); j++)
		{
			if (enemyHormingCollisions_[i]->enable_)
			{
				//Playerとキューブの当たり判定
				if (isCollisionOBBtoSphere(*enemyHormingCollisions_[i], *playerCollision_[j]))
				{
					ImGui::Begin("DEBUG COLLISION");
					ImGui::Text("Hit Cube!!!");
					ImGui::End();
				}
			}
		}
	}
}
void Object::CollisionReleaseAll()
{
	for (size_t i = 0; i < enemyHormingCollisions_.size();)
	{
		auto collision = enemyHormingCollisions_[i];
		if(collision) delete collision;
		enemyHormingCollisions_.erase(enemyHormingCollisions_.begin() + i);
	}
	for (size_t i = 0; i < playerCollision_.size();)
	{
		auto collision = playerCollision_[i];
		if(collision) delete collision;
		playerCollision_.erase(playerCollision_.begin() + i);
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
