#include "Weapon_Shotgun.h"
#include "../Player.h"
#include "../../../Camera/Camera.h"
#include "../../XModel/XModel.h"
#include "WeaponState\WeaponRelative.h"
#include "../../Shader/OutlineShader.h"
#include "../../Shader/ShadowMapShader.h"
#include "../../Shader/ToonShader.h"
#include "Bullet\Bullet_Shotgun.h"

Weapon_Shotgun::Weapon_Shotgun()
{
	//銃のモデル作成
	model_ = new XModel();
	model_->SetModelType(XModel::XMODEL::MODEL_SHOTGUN);
	relative_ = new WeaponRelative();

	//シェーダーを適用する。
	model_->SetUseShader(true);
	//親子関係を適用
	model_->SetHieral(false);

	//弾生成
	bullet_ = Object::Create<Bullet_Shotgun>();
	bullet_->SetWeapon(this);
}

Weapon_Shotgun::~Weapon_Shotgun()
{
	delete relative_;
	delete model_;
}

void Weapon_Shotgun::Init()
{
	//ワールド行列を初期化
	D3DXMatrixIdentity(&world_);
	model_->SetWorld(world_);
	//相対座標を初期化
	SetRelativePosition(-0.1f, 0.0f, 0.0f);
	//スケール初期化
	SetScale(0.05f,0.05f, 0.05f);
	//回転情報初期化
	SetRotation(0,-90.0f,0);

	model_->Init();
}

void Weapon_Shotgun::Uninit()
{
	model_->Uninit();
}

void Weapon_Shotgun::Update()
{
	model_->Update();
}

void Weapon_Shotgun::BeginDraw()
{
	//相対関係制御実行
	relative_->Act(this);

	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	model_->BeginDraw();

	//シェーダー処理
	if (model_->GetUseShader())
	{
		//シャドウマップ作成
		LPD3DXEFFECT effect;
		effect = ShadowMapShader::GetEffect();
		ShadowMapShader::SetInfo(model_->GetWorld());
		effect->SetTechnique("Shadow");
		model_->Draw(effect, 0);
	}
	
}

void Weapon_Shotgun::Draw()
{
	if (model_->GetUseShader())
	{
		LPD3DXEFFECT effect;

		//アウトライン描画
		effect = OutlineShader::GetEffect();
		OutlineShader::SetInfo(model_->GetWorld());
		effect->SetTechnique("Tech");
		model_->Draw(effect, 0);

		//トゥーンシェーダー描画
		effect = ToonShader::GetEffect();
		ToonShader::SetInfo(model_->GetWorld());
		effect->SetTechnique("ToonPaintInk");
		model_->Draw(effect, 0);
	}
	else
	{
		model_->Draw();
	}
}

void Weapon_Shotgun::EndDraw()
{
	model_->EndDraw();
}
