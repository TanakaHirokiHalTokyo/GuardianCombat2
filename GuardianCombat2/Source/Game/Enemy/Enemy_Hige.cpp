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
#include "../Cube/Cube.h"

const D3DXVECTOR3 init_pos = D3DXVECTOR3(0,0,-8.0f);

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
	
	//パラメータ初期化
	InitParameter();
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
	if (collision_)
	{
		delete collision_;
		collision_ = nullptr;
	}

	DestParameter();
}

void EnemyHige::Init()
{
	//初期化
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(init_pos);
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

	InitParameterValue();

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
	debugCollision_->SetPosition(GetPosition());
	debugCollision_->SetPositionY(debugCollision_->GetRadius());
	collision_->pos = debugCollision_->GetPosition();
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

	statePattern_->BeginDisplay();

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
		statePattern_->Display();
	}
}

void EnemyHige::EndDraw()
{
	statePattern_->EndDisplay();
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
void EnemyHige::SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER* parameter)
{
	rushParameter_ = *parameter;	//突進状態のパラメータ設定
}
EnemyHigeCircleShot::ENEMY_PARAMETER EnemyHige::GetCircleShotParameter()
{
	return circleShotParameter_;		//波状攻撃取得
}
void EnemyHige::SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER* parameter)
{
	circleShotParameter_ = *parameter;	//波状攻撃設定
}
EnemyHigeHorming::ENEMY_PARAMETER EnemyHige::GetHormingParameter()
{
	return hormingParameter_;			//ホーミングパラメータ取得
}
void EnemyHige::SetHormingParameter(EnemyHigeHorming::ENEMY_PARAMETER * parameter)
{
	hormingParameter_ = *parameter;		//ホーミングパラメータ設定
}
void EnemyHige::DrawDebug()
{
	static int listbox_item_current = 1;		//リストボックスの初期選択番号
	static bool changeState = false;			//状態を変更したか
	static bool change_cube_num = false;		//キューブの数を変更したか
	static bool reset_position = false;			//敵のポジションリセットフラグ
	static bool reset_parameter = false;		//パラメータリセットフラグ
	static bool change_hormingcube_num = false;	//ホーミングキューブの数を変更したか

	//Window位置固定
	ImGui::SetNextWindowPos(ImVec2(10,(float)ScreenHeight / 2.0f));
	//敵のデバッグ情報
	ImGui::Begin("Enemy Debug Info");
	//敵の位置表示
	ImGui::Text("Position %f %f %f",GetPosition().x, GetPosition().y, GetPosition().z);
	//ポジション初期化
	reset_position = ImGui::Button("Reset Enemy Position");
	//パラメータ初期化
	reset_parameter = ImGui::Button("Reset Parameter");
	//今現在のSTATE名
	ImGui::Text("STATE : %s",StateWord[state_]);
	//次に設定するSTATE
	changeState = ImGui::ListBox("listbox\n(single select)", &listbox_item_current, StateWord, IM_ARRAYSIZE(StateWord), 4);

	//待機状態のパラメータ設定
	if (ImGui::TreeNode("IDLE PARAMETER"))
	{
		ImGui::DragFloat("Speed Set", &idleParameter_.speed, 0.001f, 0.0f, 0.1f);			//スピード設定
		ImGui::DragFloat("RotateAngle", &idleParameter_.rot_angle, 0.01f, 0.01f, 6.0f);		//回転スピード設定
		ImGui::TreePop();
	}
	//通常攻撃パラメータ設定
	if (ImGui::CollapsingHeader("NORMAL ATTACK"))
	{
		//突進状態のパラメータ設定
		if (ImGui::TreeNode("RUSH PARAMETER"))
		{
			ImGui::DragFloat("Speed", &rushParameter_.speed, 0.01f, 0.0f, 1.0f);			//スピード設定
			ImGui::DragFloat("Length", &rushParameter_.length, 1.0f, 0.0f, 100.0f);			//突進距離設定
			ImGui::TreePop();
		}
		//波状攻撃のパラメータ設定
		if (ImGui::TreeNode("CIRCLESHOT PARAMETER"))
		{
			change_cube_num = ImGui::SliderInt("CUBE NUM", &circleShotParameter_.CUBE_NUM, 0, 20);				//キューブ数設定
			ImGui::DragFloat("InitalVelocity", &circleShotParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);		//初期速度設定
			ImGui::DragFloat("Acceleration", &circleShotParameter_.acceleration, 0.001f, 0.0f, 1.0f);			//加速度設定
			ImGui::DragFloat("Length", &circleShotParameter_.length, 1.0f, 0.0f, 100.0f);						//キューブを飛ばす距離設定
			ImGui::SliderFloat("CUBE SIZE", &circleShotParameter_.cubeSize, 0.1f, 1.0f);
			ImGui::TreePop();
		}
		//ホーミングのパラメータ設定
		if (ImGui::TreeNode("HORMING PARAMETER"))
		{
			change_hormingcube_num = ImGui::SliderInt("CUBE NUM", &hormingParameter_.CUBE_NUM, 0, 20);		//キューブ数設定
			ImGui::SliderFloat("FanAngle",&hormingParameter_.fanangle,60.0f,180.0f,"%.1f",1.0f);			//扇の角度設定
			ImGui::DragFloat("InitalVelocity", &hormingParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);		//初期速度設定
			ImGui::DragFloat("Acceleration", &hormingParameter_.acceleration, 0.001f, 0.0f, 1.0f);			//加速度設定
			ImGui::DragFloat("FanRadius",&hormingParameter_.radius,0.01f,5.0f);								//扇の半径
			ImGui::SliderInt("NextShotCoolTime",&hormingParameter_.cooltime,0,120);							//次の弾を打つまでのクールタイム
			ImGui::SliderInt("AliveTime",&hormingParameter_.alivetime,1,300);								//ホーミング生存時間
			ImGui::DragFloat("HormingAccuracy",&hormingParameter_.horming_accuracy,0.01f,0.0f,1.0f);		//ホーミング精度
			ImGui::DragFloat("SetPositionSpeed",&hormingParameter_.setposition_speed,0.01f,1.0f);			//ポジションに向かうスピード
			ImGui::DragFloat("Length", &hormingParameter_.length, 1.0f, 0.0f, 100.0f);						//キューブを飛ばす距離設定
			ImGui::SliderFloat("CUBE SIZE", &hormingParameter_.cubeSize, 0.1f, 1.0f);						//キューブのサイズ
			ImGui::TreePop();
		}
	}
	//Imugui終了
	ImGui::End();

	//状態変更したとき
	if (changeState)
	{
		//状態変更
		state_ = (STATE)listbox_item_current;
		changeState = false;
		statePattern_->ChangeState();
	}
	//キューブの数を変更したとき
	if (change_cube_num)
	{
		change_cube_num = false;

		//キューブの数が変更されていたら
		if (circleShotParameter_.CUBE_NUM != circleShotParameter_.OLD_CUBE_NUM)
		{
			//現在のキューブの数を設定
			circleShotParameter_.OLD_CUBE_NUM = circleShotParameter_.CUBE_NUM;

			delete[] circleShotParameter_.cube;
			circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];

			delete[] circleShotParameter_.vec;
			circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];

			FinishState();
		}
	}
	if (change_hormingcube_num)
	{
		change_hormingcube_num = false;

		//キューブの数が変更されていたら
		if (hormingParameter_.CUBE_NUM != hormingParameter_.OLD_CUBE_NUM)
		{
			//現在のキューブの数を設定
			hormingParameter_.OLD_CUBE_NUM = hormingParameter_.CUBE_NUM;

			delete[] hormingParameter_.cube;
			hormingParameter_.cube = new Cube[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.vec;
			hormingParameter_.vec = new Vector3[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.spawnvec;
			hormingParameter_.spawnvec = new ParameterVector[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.shot;
			hormingParameter_.shot = new bool[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.cooltimecount;
			hormingParameter_.cooltimecount = new int[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.alivetimecount;
			hormingParameter_.alivetimecount = new int[hormingParameter_.CUBE_NUM];

			FinishState();
		}
	}
	//ポジション初期化
	if (reset_position)
	{
		reset_position = false;
		SetPosition(init_pos);
	}
	//パラメータ初期化
	if (reset_parameter)
	{
		reset_parameter = false;
		//初期化用に各パラメータ作成
		EnemyIdle::ENEMY_PARAMETER idleparameter;
		EnemyHigeCircleShot::ENEMY_PARAMETER circleshotparameter;
		EnemyHigeRush::ENEMY_PARAMETER rushparameter;
		EnemyHigeHorming::ENEMY_PARAMETER hormingparameter;

		//各パラメータ初期化再設定
		SetIdleParameter(&idleparameter);
		SetRushParameter(&rushparameter);
		SetCircleShotParameter(&circleshotparameter);
		SetHormingParameter(&hormingparameter);

		InitParameter();
	}
}

void EnemyHige::InitParameter()
{
	//波状攻撃時のキューブ作成
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];
	//波状攻撃ベクトル作成
	circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];

	//ホーミングのキューブ作成
	hormingParameter_.cube = new Cube[hormingParameter_.CUBE_NUM];
	//ホーミングベクトル作成
	hormingParameter_.vec = new Vector3[hormingParameter_.CUBE_NUM];
	//ホーミングスポーンベクトル作成
	hormingParameter_.spawnvec = new ParameterVector[hormingParameter_.CUBE_NUM];
	//ホーミングショットフラグ作成
	hormingParameter_.shot = new bool[hormingParameter_.CUBE_NUM];
	//ホーミングカウンタ作成
	hormingParameter_.cooltimecount = new int[hormingParameter_.CUBE_NUM];
	//ホーミング生存時間カウンタ作成
	hormingParameter_.alivetimecount = new int[hormingParameter_.CUBE_NUM];
}

void EnemyHige::InitParameterValue()
{
	//波状攻撃初期化
	for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].SetVisible(false);
		circleShotParameter_.cube[i].SetPosition(0.0f, 0.0f, 0.0f);
		circleShotParameter_.cube[i].SetScale(circleShotParameter_.cubeSize, circleShotParameter_.cubeSize, circleShotParameter_.cubeSize);
	}

	//ホーミング初期化
	for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.cube[i].SetVisible(false);
		hormingParameter_.cube[i].SetPosition(0.0f,0.0f,0.0f);
		hormingParameter_.cube[i].SetScale(hormingParameter_.cubeSize, hormingParameter_.cubeSize, hormingParameter_.cubeSize);
		hormingParameter_.shot[i] = false;
		hormingParameter_.cooltimecount[i] = 0;
		hormingParameter_.alivetimecount[i] = 0;
	}
}

void EnemyHige::DestParameter()
{
	if (circleShotParameter_.cube)
	{
		delete[] circleShotParameter_.cube;
	}
	if (circleShotParameter_.vec)
	{
		delete[] circleShotParameter_.vec;
	}
	if (hormingParameter_.cube)
	{
		delete[] hormingParameter_.cube;
	}
	if (hormingParameter_.vec)
	{
		delete[] hormingParameter_.vec;
	}
	if (hormingParameter_.spawnvec)
	{
		delete[] hormingParameter_.spawnvec;
	}
	if (hormingParameter_.shot)
	{
		delete[] hormingParameter_.shot;
	}
	if (hormingParameter_.cooltimecount)
	{
		delete[] hormingParameter_.cooltimecount;
	}
	if (hormingParameter_.alivetimecount)
	{
		delete[] hormingParameter_.alivetimecount;
	}
}
