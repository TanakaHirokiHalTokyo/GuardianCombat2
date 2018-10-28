#include "Core.h"
#include "../XModel/XModel.h"
#include "StateCore\StateCore.h"
#include "StateCore\StateCore_Default.h"
#include "../../Imgui/ImguiManager.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ShadowMapShader.h"
#include "../Shader/ToonShader.h"
#include "../Billboard/Billboard.h"
#include "../../Effekseer/Effekseer.h"

Core::Core()
{
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::MODE_CORE);
	model_->SetUseShader(true);
	coreEffect_ =new CEffekseer(CEffekseer::Effect_Core);
	coreEffect_->Init();
	coreEffect_->RepeatEffect(true);
	coreEffect_->Play();
	stateCore_ = new StateCore_Default();
}

Core::~Core()
{
	if (stateCore_ != nullptr)
	{
		delete stateCore_;
		stateCore_ = nullptr;
	}
	if (coreEffect_ != nullptr)
	{
		coreEffect_->Uninit();
		delete coreEffect_;
		coreEffect_ = nullptr;
	}
}

void Core::Init()
{
	SetPosition(0.0f, 1.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(0.2f, 0.2f, 0.2f);
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());
	model_->SetScale(GetScale());
	model_->SetUseShader(true);
	coreEffect_->SetPosition(GetPosition().x,0.0f,GetPosition().z);
	coreEffect_->SetVisible(true);
	coreEffect_->SetScale(0.2f, 0.2f, 0.2f);
	coreEffect_->SetSpeed(1.5f);
}

void Core::Uninit()
{
}

void Core::Update()
{
	if (stateCore_ != nullptr)
	{
		stateCore_->UpdateCore(this);
	}

	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());
	model_->SetScale(GetScale());

	coreEffect_->Update();
}

void Core::BeginDraw()
{
	D3DXMATRIX trans, rotX, rotY, rotZ, scale;
	D3DXMatrixTranslation(&trans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixRotationX(&rotX, D3DXToRadian(transform_.rotate.x));
	D3DXMatrixRotationX(&rotY, D3DXToRadian(transform_.rotate.y));
	D3DXMatrixRotationX(&rotZ, D3DXToRadian(transform_.rotate.z));
	D3DXMatrixScaling(&scale, transform_.scale.x, transform_.scale.y, transform_.scale.z);

	D3DXMatrixIdentity(&world_);

	world_ = scale;
	world_ *= rotX;
	world_ *= rotY;
	world_ *= rotZ;
	world_ *= trans;

	if (model_->GetUseShader())
	{
		//ShadowMapCreate
		LPD3DXEFFECT effect;
		effect = ShadowMapShader::GetEffect();
		ShadowMapShader::SetInfo(world_);
		effect->SetTechnique("Shadow");
		model_->Draw(effect,0);
	}
}

void Core::Draw()
{
	if (model_->GetUseShader())
	{
		LPD3DXEFFECT effect;

		//OutlineShaderDraw
		effect = OutlineShader::GetEffect();
		OutlineShader::SetInfo(world_);
		effect->SetTechnique("Tech");
		model_->Draw(effect,0);

		//ToonShaderDraw
		effect = ToonShader::GetEffect();
		ToonShader::SetInfo(world_);
		effect->SetTechnique("ToonPaintInk");
		model_->Draw(effect,0);
	}

	ImGui::Begin("Core Info"); 
	ImGui::Text("Position : %f %f %f",GetPosition().x, GetPosition().y, GetPosition().z);
	ImGui::End();

	coreEffect_->Draw();
}

void Core::EndDraw()
{
}
