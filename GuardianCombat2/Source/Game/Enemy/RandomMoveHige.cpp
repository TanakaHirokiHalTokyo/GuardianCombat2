#include "RandomMoveHige.h"
#include "../XModel/XModel.h"
#include "../Shader/ToonShader.h"
#include "../Shader/OutlineShader.h"
#include "../../Random/Random.h"
#include "../../Vector3/Vector3.h"
#include "../../Imgui/ImguiManager.h"
#include "../Shader/ShadowMapShader.h"
#include "../../main.h"

const float ENEMY_SPEED = 0.05f;								//敵のスピード
const float ENEMY_ROT_SPEED = 1.0f;						//敵の回転速度
const int NEXT_POSITION_TIMER = GameFPS * 5;		//次の地点を算出するタイマー

RandomMoveHige::RandomMoveHige()
{
	//Model作成
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::MODEL_HIGE);

	//ベクトル作成
	enemy_vector_ = new Vector3();
}

RandomMoveHige::~RandomMoveHige()
{
	SAFE_DELETE(enemy_vector_);
}

void RandomMoveHige::Init()
{
	model_->SetUseShader(true);
	model_->SetPosition(-0.1f, 0.0f, -8.0f);
	model_->SetScale(0.3f,0.3f,0.3f);
}

void RandomMoveHige::Uninit()
{
}

void RandomMoveHige::Update()
{
	timer_++;

	if (length_ <= 2.0f || timer_ >= NEXT_POSITION_TIMER)
	{
		timer_ = 0;
		float randomX, randomY, randomZ;
		randomX = GC2::Random::RangeFloat(rangeX_.min,rangeX_.max);
		randomY = GC2::Random::RangeFloat(rangeY_.min, rangeY_.max);
		randomZ = GC2::Random::RangeFloat(rangeZ_.min, rangeZ_.max);

		next_position_ = D3DXVECTOR3(randomX,randomY,randomZ);
	}
	Rotate();
	D3DXVECTOR3 front = enemy_vector_->GetFront();
	
	model_->SetPosition(model_->GetPosition() + front * ENEMY_SPEED);

	D3DXVECTOR3 leng_vec = next_position_ - model_->GetPosition();
	length_ = D3DXVec3Length(&leng_vec);

	SetPosition(model_->GetPosition());
}

void RandomMoveHige::BeginDraw()
{
	//ワールド行列計算
	D3DXMATRIX trans, rotX, rotY, rotZ, scale;
	D3DXMatrixTranslation(&trans, model_->GetPosition().x, model_->GetPosition().y, model_->GetPosition().z);
	D3DXMatrixRotationX(&rotX, D3DXToRadian(model_->GetRotate().x));
	D3DXMatrixRotationY(&rotY, D3DXToRadian(model_->GetRotate().y));
	D3DXMatrixRotationZ(&rotZ, D3DXToRadian(model_->GetRotate().z));
	D3DXMatrixScaling(&scale, model_->GetScale().x, model_->GetScale().y, model_->GetScale().z);

	D3DXMatrixIdentity(&world_);

	world_ = scale;
	world_ *= rotX;
	world_ *= rotY;
	world_ *= rotZ;
	world_ *= trans;

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

void RandomMoveHige::Draw()
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
		effect->SetTechnique("ToonPaint");
		model_->Draw(effect, 0);
	}
}

void RandomMoveHige::EndDraw()
{
}

void RandomMoveHige::Rotate()
{
	auto front = enemy_vector_->GetFront();
	auto right = enemy_vector_->GetRight();
	auto up = enemy_vector_->GetUp();

	D3DXVECTOR3 vector = next_position_ - model_->GetPosition();

	float dot = D3DXVec3Dot(&front,	&vector);
	float right_dot = D3DXVec3Dot(&right,&vector);
	float angle = D3DXToDegree(acosf(dot));

	if (isnan(angle))angle = 0.0f;

	if (right_dot >= 0.0f)
	{
		model_->SetRotationY(model_->GetRotate().y - ENEMY_ROT_SPEED);

		//回転行列を作成
		D3DXMATRIX mtxRotate;
		D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(-ENEMY_ROT_SPEED));
		D3DXVec3TransformNormal(&front, &front, &mtxRotate);
		D3DXVec3Normalize(&front, &front);
		enemy_vector_->SetFront(front);
		D3DXVec3Cross(&right, &enemy_vector_->GetFront(), &enemy_vector_->GetUp());
		D3DXVec3Normalize(&right, &right);
		enemy_vector_->SetRight(right);
	}
	else
	{
		model_->SetRotationY(model_->GetRotate().y + ENEMY_ROT_SPEED);

		//回転行列を作成
		D3DXMATRIX mtxRotate;
		D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(ENEMY_ROT_SPEED));
		D3DXVec3TransformNormal(&front, &front, &mtxRotate);
		D3DXVec3Normalize(&front, &front);
		enemy_vector_->SetFront(front);
		D3DXVec3Cross(&right, &enemy_vector_->GetFront(), &enemy_vector_->GetUp());
		D3DXVec3Normalize(&right, &right);
		enemy_vector_->SetRight(right);
	}
}
