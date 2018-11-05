#include "Enemy_Hige.h"
#include "../XModel/XModel.h"
#include "../Shader/ToonShader.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ShadowMapShader.h"
#include "../../Vector3/Vector3.h"
#include "../../Imgui/ImguiManager.h"
#include "StateEnemy\StatePattern_EnemyHige.h"
#include "../../Collision/Collision.h"
#include "../../Debug/Debug.h"
#include "../../main.h"

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

	//コリジョン作成
	collision_ = new Sphere();
	collision_->object_ = this;
	collision_->pos = GetPosition();

	//コリジョン可視化作成
	debugCollision_ = Object::Create<DebugSphere>();

}

EnemyHige::~EnemyHige()
{
	if (vector_)
	{
		delete vector_;
		vector_ = nullptr;
	}
	
	if (statePattern_ )
	{
		delete statePattern_;
		statePattern_ = nullptr;
	}
}

void EnemyHige::Init()
{
	//初期化
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(0,0,-8.0f);
	SetRotation(0,0,0);
	SetSpeed(0.02f);

	//モデル情報初期化
	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	//ベクトル初期化
	vector_->SetFront(0,0,1.0f);
	vector_->SetRight(1.0f,0.0f,0.0f);
	vector_->SetUp(0,1.0f,0);

	//Collision初期化
	collision_->rad = 0.5f;

	//デバッグ表示のラディウス設定
	debugCollision_->SetRadius(collision_->rad);

	//デバッグモードON
	debug_ = true;
}

void EnemyHige::Uninit()
{
}

void EnemyHige::Update()
{
	//ステート制御実行
	statePattern_->Act();

	//モデル情報更新
	model_->SetRotation(GetRotate());
	model_->SetPosition(GetPosition());
	model_->SetScale(GetScale());

	//コリジョン更新
	collision_->pos = GetPosition();
	debugCollision_->SetPosition(GetPosition());
	debugCollision_->SetPositionY(debugCollision_->GetRadius());
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

	if (debug_)
	{
		//デバッグ表示　ステート・パラメータ表示
		DrawDebug();
	}
}

void EnemyHige::EndDraw()
{
	
}

EnemyHige::STATE EnemyHige::GetState()
{
	return state_;		//状態取得
}

void EnemyHige::SetState(STATE state)
{
	state_ = state;		//状態設定
}
void EnemyHige::FinishState()
{
	state_ = IDLE;
	statePattern_->ChangeState();
}
EnemyHigeRush::ENEMY_PARAMETER EnemyHige::GetRushParameter()
{
	return rushParameter_;		//突進状態のパラメータ取得
}
void EnemyHige::SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER parameter)
{
	rushParameter_ = parameter;	//突進状態のパラメータ設定
}
void EnemyHige::DrawDebug()
{
	static int listbox_item_current = 1;
	static bool changeState = false;

	//Window位置固定
	ImGui::SetNextWindowPos(ImVec2(10,(float)ScreenHeight / 2.0f));
	//敵のデバッグ情報
	ImGui::Begin("Enemy Debug Info");
	//今現在のSTATE名
	ImGui::Text("STATE : %s",StateWord[state_]);
	//次に設定するSTATE
	changeState = ImGui::ListBox("listbox\n(single select)", &listbox_item_current, StateWord, IM_ARRAYSIZE(StateWord), 4);

	//待機状態のパラメータ設定
	if (ImGui::TreeNode("IDLE PARAMETER"))
	{
		ImGui::DragFloat("Speed Set", &idleParameter_.speed, 0.001f, 0.0f, 0.1f);
		ImGui::DragFloat("RotateAngle", &idleParameter_.rot_angle, 0.1f, 0.0f, 10.0f);
		ImGui::TreePop();
	}
	//通常攻撃パラメータ設定
	if (ImGui::CollapsingHeader("NORMAL ATTACK"))
	{
		//突進状態のパラメータ設定
		if (ImGui::TreeNode("RUSH PARAMETER"))
		{
			ImGui::DragFloat("Speed Set", &rushParameter_.speed, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Length Set", &rushParameter_.length, 1.0f, 0.0f, 100.0f);
			ImGui::TreePop();
		}
	}
	//Imugui終了
	ImGui::End();

	if (changeState)
	{
		//状態変更
		state_ = (STATE)listbox_item_current;
		changeState = false;
		statePattern_->ChangeState();
	}

	statePattern_->Display();
}