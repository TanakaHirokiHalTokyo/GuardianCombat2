#include "Weapon_Shotgun.h"
#include "../Player.h"
#include "../../../Camera/Camera.h"
#include "../../XModel/XModel.h"
#include "WeaponState\WeaponRelative.h"
#include "../../Shader/OutlineShader.h"
#include "../../Shader/ShadowMapShader.h"
#include "../../Shader/ToonShader.h"
#include "Bullet\Bullet_Shotgun.h"
#include "../../../DInput/DirectInput.h"
#include "../../../XInput/xcontroller.h"
#include "../../../main.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Sound/Sound.h"

Weapon_Shotgun::Weapon_Shotgun()
{
	//銃のモデル作成
	model_ = new XModel();
	model_->SetModelType(XModel::XMODEL::MODEL_SHOTGUN);
	relative_ = new WeaponRelative();

	//シェーダーを適用する。
	model_->SetUseShader(true);

	//弾生成
	bullet_ = new Bullet_Shotgun[BulletNum];
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].SetWeapon(this);
		bullet_[i].Init();
	}

	shotSE_ = new Sound(SoundManager::BULLETSHOT_SE);
}

Weapon_Shotgun::~Weapon_Shotgun()
{
	if (relative_)
	{
		delete relative_;
		relative_ = nullptr;
	}
	if (model_) {
		model_->Uninit();
		delete model_;
		model_ = nullptr;
	}
	if (shotSE_)
	{
		shotSE_->StopSound();
		delete shotSE_;
		shotSE_ = nullptr;
	}

	if (bullet_)
	{
		delete[] bullet_;
		bullet_ = nullptr;
	}
}

void Weapon_Shotgun::Init()
{
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Init();
		bullet_[i].SetVisible(false);
	}

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
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Uninit();
	}
	model_->Uninit();
}

void Weapon_Shotgun::Update()
{
	DIMOUSESTATE mouse = ReturnMouseMove();

	size_t rate_count = 0;
	if (fireRate_ > 0.0f) { 
		rate_count = (size_t)((float)GameFPS / fireRate_); }

	if (mouse.rgbButtons[0] == 0x80 || X_CONTROLLER::GetXcontrollerButtonPress(1, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		rateCount_++;
		if (rateCount_ >= rate_count)
		{
			shotSE_->PlaySoundA();
			ShotBullet(BulletNum);
			rateCount_ = 0;
		}
	}
	//相対関係制御実行
	relative_->Act(this);

	model_->Update();

	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Update();
	}
}

void Weapon_Shotgun::BeginDraw()
{
	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	model_->BeginDraw();


	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].BeginDraw();
	}

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
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Draw();
	}
}

void Weapon_Shotgun::EndDraw()
{
	model_->EndDraw();
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].EndDraw();
	}
}
