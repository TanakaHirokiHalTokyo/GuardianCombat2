#include "Enemy_HigeAvater.h"
#include "../XModel/XModel.h"
#include "StateEnemy\StateEnemy_Rotate.h"
#include "Enemy_Hige.h"
#include "../Shader/ShadowMapShader.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ToonShader.h"
#include "../../Vector3/Vector3.h"
#include "../GameManager/GameManager.h"
#include "../../Game/Player/Player.h"

EnemyHige_Avater::EnemyHige_Avater()
{
	//モデル作成
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::MODEL_HIGE);
	model_->SetUseShader(true);

	//ベクトル作成
	vector_ = new Vector3();

	//初期化
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(0.0f,0.0f,0.0f);
	SetRotation(0, 0, 0);
	SetSpeed(0.00f);

	//モデル情報初期化
	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	//回転制御作成
	rotate_ = new StateEnemy_Rotate();
}

EnemyHige_Avater::~EnemyHige_Avater()
{
	if (rotate_)
	{
		delete rotate_;
		rotate_ = nullptr;
	}
}

void EnemyHige_Avater::Init()
{
	//初期化
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(0.0f,0.0f,0.0f);
	SetRotation(0, 0, 0);
	SetSpeed(0.0f);
	
	//進んだ距離リセット
	distance_ = 0.0f;
}

void EnemyHige_Avater::Uninit()
{
}

void EnemyHige_Avater::Update()
{
	if (GetVisible())
	{
		rotate_->Act(this);
	}
}

void EnemyHige_Avater::BeginDraw()
{
	if (GetVisible())
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

		rotate_->BeginDisplay(this);

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
}

void EnemyHige_Avater::Draw()
{
	if (GetVisible())
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
		rotate_->Display(this);
	}
	
}

void EnemyHige_Avater::EndDraw()
{
	if (GetVisible())
	{
		rotate_->EndDisplay(this);
	}
}

void EnemyHige_Avater::SetSpawnVector(D3DXVECTOR3 vec)
{
	spawnVec_ = vec;	
}

void EnemyHige_Avater::SetSpawnVector(float x, float y, float z)
{
	spawnVec_ = D3DXVECTOR3(x,y,z);
}

D3DXVECTOR3 EnemyHige_Avater::GetSpawnVector()
{
	return spawnVec_;
}
