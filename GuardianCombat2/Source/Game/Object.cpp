#include "Object.h"
#include "../main.h"
#define DEBUG_PLAYER	(false)


vector<Object*> Object::objects_;


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
void Object::UpdateAll()
{
	for (unsigned int i = 0; i < objects_.size();)
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
	for (unsigned int i = 0; i < objects_.size(); i++)
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
	for (unsigned int i = 0; i < objects_.size(); ++i)
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
	for (unsigned int i = 0; i < objects_.size(); i++)
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
	for (unsigned int i = 0; i < objects_.size(); )
	{
		auto obj = objects_[i];
		obj->Uninit();
		SAFE_DELETE(obj);
		objects_.erase(objects_.begin() + i);

	}
}
void Object::JudgementAll()
{
	
}
void Object::CollisionReleaseAll()
{
	
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
