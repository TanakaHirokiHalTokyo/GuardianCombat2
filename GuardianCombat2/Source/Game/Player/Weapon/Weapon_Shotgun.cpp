#include "Weapon_Shotgun.h"
#include "../Player.h"
#include "../../../Camera/Camera.h"
#include "../../XModel/XModel.h"

Weapon_Shotgun::Weapon_Shotgun()
{
	//�e�̃��f���쐬
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::XMODEL::MODEL_SHOTGUN);
}

Weapon_Shotgun::~Weapon_Shotgun()
{
}

void Weapon_Shotgun::Init()
{
	//���[���h�s���������
	D3DXMatrixIdentity(&world_);
	//���΍��W��������
	SetPosition(0.1f,0.1f,0.0f);
	//�X�P�[��������
	SetScale(0.1f,0.1f,0.1f);
	//��]��񏉊���
	SetRotation(0,0,0);

	//Model���ɔ��f
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
