#include "UI.h"
#include "../main.h"
#include "../Imgui/ImguiManager.h"
#include "UIState\UIState_Move.h"
#include "UIState\UIState_Scale.h"

UI::UI(TextureManager::TextureList type)
{
	//Texture生成
	texture_ = Object::Create<Texture>(type);
}

UI::~UI()
{
	if (stateMove_ != nullptr)
	{
		delete stateMove_;
		stateMove_ = nullptr;
	}
	if (stateScale_ != nullptr)
	{
		delete stateScale_;
		stateScale_ = nullptr;
	}
}

void UI::Init()
{
	if(stateMove_ != nullptr) stateMove_->Init();										//移動制御初期化
	if (stateScale_ != nullptr) stateScale_->Init();									//サイズ制御初期化
	SetPosition(startPos_.x,startPos_.y,0);					//開始地点初期化
	SetScale(startScale_.x, startScale_.y,0);				//サイズ初期化
	texture_->SetPosition(GetPosition());					//テクスチャの座標更新
	texture_->SetDrawSize(GetScale().x,GetScale().y);		//テクスチャのサイズ更新
}

void UI::Uninit()
{
}

void UI::Update()
{
	if (stateMove_ != nullptr) { stateMove_->Update(this); }			//ポジション更新
	if (stateScale_ != nullptr) { stateScale_->Update(this); }			//サイズ更新

	texture_->SetPosition(GetPosition());
	texture_->SetDrawSize(GetScale().x, GetScale().y);
}

void UI::BeginDraw()
{
}

void UI::Draw()
{
	
}

void UI::EndDraw()
{
}

void UI::MoveTexture(float afterSpawnCount, float moveTime, float update_x, float update_y)
{
	if (stateMove_ == nullptr) { stateMove_ = new UIState_Move(); }
	else { delete stateMove_; stateMove_ = new UIState_Move(); }						//更新途中に別の動きをさせたい場合new しなおす

	this->countMove_ = afterSpawnCount;
	this->moveTime_ = moveTime;
	this->updatePos_.x = update_x;
	this->updatePos_.y = update_y;
}

void UI::MoveTexture(float afterSpawnCount, float moveTime, D3DXVECTOR3 update_pos)
{
	if (stateMove_ == nullptr){	stateMove_ = new UIState_Move();}
	else { delete stateMove_; stateMove_ = new UIState_Move(); }						//更新途中に別の動きをさせたい場合new しなおす

	this->countMove_ = afterSpawnCount;
	this->moveTime_ = moveTime;
	this->updatePos_.x = update_pos.x;
	this->updatePos_.y = update_pos.y;
}

void UI::MoveTexture(float afterSpawnCount, float moveTime, D3DXVECTOR2 update_pos)
{
	if (stateMove_ == nullptr) { stateMove_ = new UIState_Move(); }
	else { delete stateMove_; stateMove_ = new UIState_Move(); }						//更新途中に別の動きをさせたい場合new しなおす

	this->countMove_ = afterSpawnCount;
	this->moveTime_ = moveTime;
	this->updatePos_.x = update_pos.x;
	this->updatePos_.y = update_pos.y;
}

void UI::ScalingTexture(float afterSpawnCount, float moveTime, float update_x, float update_y)
{
	if (stateScale_ == nullptr){ stateScale_ = new UIState_Scale();}
	else { delete stateScale_; stateScale_ = new UIState_Scale(); }

	this->countScale_ = afterSpawnCount;
	this->scaleTime_ = moveTime;
	this->updateSize_.x = update_x;
	this->updateSize_.y = update_y;
}

void UI::ScalingTexture(float afterSpawnCount, float moveTime, D3DXVECTOR3 update_scale)
{
	if (stateScale_ == nullptr) { stateScale_ = new UIState_Scale(); }
	else { delete stateScale_; stateScale_ = new UIState_Scale(); }

	this->countScale_ = afterSpawnCount;
	this->scaleTime_ = moveTime;
	this->updateSize_.x = update_scale.x;
	this->updateSize_.y = update_scale.y;
}

void UI::ScalingTexture(float afterSpawnCount, float moveTime, D3DXVECTOR2 update_scale)
{
	if (stateScale_ == nullptr) { stateScale_ = new UIState_Scale(); }
	else { delete stateScale_; stateScale_ = new UIState_Scale(); }

	this->countScale_ = afterSpawnCount;
	this->scaleTime_ = moveTime;
	this->updateSize_.x = update_scale.x;
	this->updateSize_.y = update_scale.y;
}

Texture * UI::GetTexture()
{
	return texture_;
}

float UI::GetCountMove()
{
	return countMove_;
}

float UI::GetMoveTime()
{
	return moveTime_;
}

D3DXVECTOR2 UI::GetAfterPos()
{
	return updatePos_;
}

float UI::GetCountScale()
{
	return countScale_;
}

float UI::GetScaleTime()
{
	return scaleTime_;
}

D3DXVECTOR2 UI::GetAfterScale()
{
	return updateSize_;
}

void UI::SetStartPosition(D3DXVECTOR2 pos)
{
	startPos_ = pos;
}

void UI::SetStartPosition(float x, float y)
{
	startPos_ = D3DXVECTOR2(x,y);
}

D3DXVECTOR2 UI::GetStartPosition()
{
	return startPos_;
}

void UI::SetStartScale(D3DXVECTOR2 scale)
{
	startScale_ = scale;
}

void UI::SetStartScale(float x, float y)
{
	startScale_ = D3DXVECTOR2(x,y);
}

D3DXVECTOR2 UI::GetStartScale()
{
	return startScale_;
}
