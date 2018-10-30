#include "Weapon_Shotgun.h"
#include "../Player.h"
#include "../../../Camera/Camera.h"
#include "../../XModel/XModel.h"

Weapon_Shotgun::Weapon_Shotgun()
{
	//銃のモデル作成
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::XMODEL::MODEL_SHOTGUN);
}

Weapon_Shotgun::~Weapon_Shotgun()
{
}

void Weapon_Shotgun::Init()
{
	//ワールド行列を初期化
	D3DXMatrixIdentity(&world_);
	//相対座標を初期化
	SetPosition(0.1f,0.1f,0.0f);
	//スケール初期化
	SetScale(0.1f,0.1f,0.1f);
	//回転情報初期化
	SetRotation(0,0,0);

	//Model情報に反映
	model_->SetPosition(GetPosition());
	model_->SetScale(GetScale());
	model_->SetRotation(GetRotate());
}

void Weapon_Shotgun::Uninit()
{
}

void Weapon_Shotgun::Update()
{
}

void Weapon_Shotgun::BeginDraw()
{
}

void Weapon_Shotgun::Draw()
{
}

void Weapon_Shotgun::EndDraw()
{
}
