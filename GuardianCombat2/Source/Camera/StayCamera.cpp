#include "StayCamera.h"
#include "../Vector3/Vector3.h"
#include "../Imgui/ImguiManager.h"

StayCamera::StayCamera()
{
	vector_ = new Vector3();
	vector_->SetFront(0.0f,0.0f,1.0f);
	vector_->SetUp(0.0f,1.0f,0.0f);
	vector_->SetRight(-1.0f,0.0f,0.0f);
	SetPosition(0.0f, 1.0f, -10.0f);
	at_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->SetProjection();
}

StayCamera::~StayCamera()
{
	if (vector_ != nullptr)
	{
		delete vector_;
		vector_ = nullptr;
	}
}

void StayCamera::Init()
{
	
}

void StayCamera::Uninit()
{
}

void StayCamera::Update()
{
	this->vector_->SetFront(GetAt() - GetPosition());
	//右ベクトル
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);

	this->SetView();
	this->SetMainCameraInfo(this->transform_.pos, this->at_, vector_->GetUp(), this->mtxView_, this->mtxProj_);
}

void StayCamera::BeginDraw()
{
}

void StayCamera::Draw()
{

}

void StayCamera::EndDraw()
{
}