#include "Enemy_Hige.h"
#include "../XModel/XModel.h"
#include "../Shader/ToonShader.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ShadowMapShader.h"
#include "../../Vector3/Vector3.h"
#include "../../Imgui/ImguiManager.h"
#include "StateEnemy\StatePattern_EnemyHige.h"

EnemyHige::EnemyHige()
{
	//モデル作成
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::XMODEL::MODEL_HIGE);
	model_->SetUseShader(true);

	//ベクトル作成
	vector_ = new Vector3();

	//ステート管理
	statePattern_ = new StatePattern_EnemyHige(this);
}

EnemyHige::~EnemyHige()
{
	delete vector_;
	if (statePattern_ )
	{
		delete statePattern_;
	}
}

void EnemyHige::Init()
{
	//初期化
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(0,0,-8.0f);
	SetRotation(0,0,0);
	SetSpeed(0.02f);

	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	vector_->SetFront(0,0,1.0f);
	vector_->SetRight(1.0f,0.0f,0.0f);
	vector_->SetUp(0,1.0f,0);
}

void EnemyHige::Uninit()
{
}

void EnemyHige::Update()
{
	//ステート制御実行
	statePattern_->Act();

	model_->SetRotation(GetRotate());
	model_->SetPosition(GetPosition());
	model_->SetScale(GetScale());
}

void EnemyHige::BeginDraw()
{
	//ワールド行列計算
	D3DXMATRIX trans, rotX, rotY, rotZ, scale;
	D3DXMatrixTranslation(&trans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixRotationX(&rotX, D3DXToRadian(transform_.rotate.x));
	D3DXMatrixRotationY(&rotY, D3DXToRadian(transform_.rotate.y));
	D3DXMatrixRotationZ(&rotZ, D3DXToRadian(transform_.rotate.z));
	D3DXMatrixScaling(&scale, transform_.scale.x, transform_.scale.y, transform_.scale.z);

	D3DXMatrixIdentity(&world_);

	world_ = scale;
	world_ *= rotX;
	world_ *= rotY;
	world_ *= rotZ;
	world_ *= trans;

	SetWorld(&world_);

	//シェーダー処理
	if (model_->GetUseShader())
	{	
		//シャドウマップ作成
		LPD3DXEFFECT effect;
		effect = ShadowMapShader::GetEffect();
		ShadowMapShader::SetInfo(world_);
		effect->SetTechnique("Shadow");
		model_->Draw(effect, 0);
	}
}

void EnemyHige::Draw()
{
	if (model_->GetUseShader())
	{
		LPD3DXEFFECT effect;

		//アウトライン描画
		effect = OutlineShader::GetEffect();
		OutlineShader::SetInfo(world_);
		effect->SetTechnique("Tech");
		model_->Draw(effect, 0);

		//トゥーンシェーダー描画
		effect = ToonShader::GetEffect();
		ToonShader::SetInfo(world_);
		effect->SetTechnique("ToonPaintInk");
		model_->Draw(effect, 0);
	}
}

void EnemyHige::EndDraw()
{
}

EnemyHige::STATE EnemyHige::GetState()
{
	return state_;
}

void EnemyHige::SetState(STATE state)
{
	state_ = state;
}
