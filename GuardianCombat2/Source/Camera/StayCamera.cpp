#include "StayCamera.h"
#include "../Vector3/Vector3.h"
#include "../Imgui/ImguiManager.h"

StayCamera::StayCamera()
{
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
	this->SetView();
	this->SetMainCameraInfo(this->transform_.pos, this->at_, vector_->GetUp(), this->mtxView_, this->mtxProj_);

	ImGui::Begin("Debug Camera");
	ImGui::SliderFloat("Pos X",&transform_.pos.x,-300.0f,300.0f);
	ImGui::SliderFloat("Pos Y", &transform_.pos.y, -300.0f, 300.0f);
	ImGui::SliderFloat("Pos Z", &transform_.pos.z, -300.0f, 300.0f);
	ImGui::End();
}

void StayCamera::BeginDraw()
{
}

void StayCamera::Draw()
{
	ImGui::Begin("Debug Camera");
	ImGui::Text("Pos %f %f %f",transform_.pos.x,transform_.pos.y,transform_.pos.z);
	ImGui::Text("At %f %f %f", at_.x, at_.y, at_.z);
	ImGui::End();
}

void StayCamera::EndDraw()
{
}

void StayCamera::SetModel1(XModel * model)
{
	model1_ = model;
}

void StayCamera::SetModel2(XModel * model)
{
	model2_ = model;
}
