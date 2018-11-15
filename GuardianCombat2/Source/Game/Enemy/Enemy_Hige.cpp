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
#include "../Player/Player.h"
#include "../Effect/Effect.h"

const D3DXVECTOR3 init_pos = D3DXVECTOR3(-0.1f,0.0f,-8.0f);

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

	ring_ = Object::Create<XModel>();
	ring_->SetModelType(XModel::MODEL_RING);
	ring_->SetScale(0.5f, 0.5f, 0.5f);
	
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
	Player* player = GameManager::GetPlayer();
	D3DXVECTOR3 front = player->GetPosition() - GetPosition();
	D3DXVec3Normalize(&front,&front);
	vector_->SetFront(front);
	D3DXVECTOR3 right;
	//右ベクトル
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);
	vector_->SetUp(0,1.0f,0);

	//Collision初期化
	collision_->rad = 0.5f;

	//波状パラメータ初期化
	InitCircleParameterValue();
	//ホーミングパラメータ初期化
	InitHormingParameterValue();

	//デバッグモードON
	debug_ = true;
}

void EnemyHige::Uninit()
{
}

void EnemyHige::Update()
{
	//敵だけのポーズモード適用するのか判断
	JudgePause();

	if (!enemypause_)
	{
		//ステート制御実行
		statePattern_->Act();

		//モデル情報更新
		model_->SetRotation(GetRotate());
		model_->SetPosition(GetPosition());
		model_->SetScale(GetScale());

		ring_->SetPosition(GetPosition());
		ring_->SetPositionY(ring_->GetPosition().y + ring_->GetScale().x);
		collision_->pos = GetPosition();
		collision_->pos.y = collision_->pos.y + collision_->rad;
	}
	else
	{
		if (debug_)
		{
			ImGui::Begin("Enemy Debug Info");
			ImGui::Text("EnemyPausing");
			ImGui::End();
		}
	}
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
	}
	statePattern_->Display();
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
	static int listbox_item_current = 1;			//リストボックスの初期選択番号
	static bool changeState = false;				//状態を変更したか
	static bool reset_position = false;				//敵のポジションリセットフラグ
	static bool reset_parameter = false;			//パラメータリセットフラグ

	//Window位置固定
	//ImGui::SetNextWindowPos(ImVec2(10,0.0f));
	//敵のデバッグ情報
	ImGui::Begin("Enemy Debug Info");
	//敵の位置表示
	ImGui::Text("Position %f %f %f",GetPosition().x, GetPosition().y, GetPosition().z);
	//敵のベクトル表示
	ImGui::Text("FrontVector\t %f %f %f",vector_->GetFront().x, vector_->GetFront().y, vector_->GetFront().z);
	ImGui::Text("RightVector\t %f %f %f", vector_->GetRight().x, vector_->GetRight().y, vector_->GetRight().z);
	ImGui::Text("UpVector\t %f %f %f", vector_->GetUp().x, vector_->GetUp().y, vector_->GetUp().z);
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
		//===========================================================================================================
		//		CircleShot
		//=======================================================================================================
		{
			static bool change_cube_num = false;			//キューブの数を変更したか
			static bool change_effect_limit = true;			//エフェクトの生存時間を変更したか
			static int effect_limit = 90;								//エフェクトの生存時間
			static bool change_effect_color = false;			//エフェクトの色を変更したか
			static float effect_color[3];								//エフェクトの色
			static float effect_size = 1.0f;							//エフェクトのサイズ
			static bool change_effect_size = false;			//エフェクトのサイズを変更したか

			//波状攻撃のパラメータ設定
			if (ImGui::TreeNode("CIRCLESHOT PARAMETER"))
			{
				change_cube_num = ImGui::SliderInt("CUBE NUM", &circleShotParameter_.CUBE_NUM, 1, 20);				//キューブ数設定
				ImGui::DragFloat("InitalVelocity", &circleShotParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);						//初期速度設定
				ImGui::DragFloat("Acceleration", &circleShotParameter_.acceleration, 0.001f, 0.0f, 1.0f);							//加速度設定
				ImGui::DragFloat("Length", &circleShotParameter_.length, 1.0f, 0.0f, 100.0f);												//キューブを飛ばす距離設定
				ImGui::SliderFloat("CUBE SIZE", &circleShotParameter_.cubeSize, 0.1f, 1.0f);											//キューブのサイズ
				change_effect_color = ImGui::ColorEdit3("EffectColor",effect_color);															//エフェクトのカラー設定
				change_effect_size = ImGui::DragFloat("Effect Size",&effect_size,0.1f,0.0f,10.0f);											//エフェクトサイズの設定
				change_effect_limit = ImGui::SliderInt("Effect Limit",&effect_limit,0,150);													//エフェクトの生存時間設定

				ImGui::TreePop();
			}
			//キューブの数を変更したとき
			if (change_cube_num)
			{
				change_cube_num = false;

				//キューブの数が変更されていたら
				if (circleShotParameter_.CUBE_NUM != circleShotParameter_.OLD_CUBE_NUM)
				{
					change_effect_color = true;
					change_effect_limit = true;
					change_effect_size = true;

					//現在のキューブの数を設定
					circleShotParameter_.OLD_CUBE_NUM = circleShotParameter_.CUBE_NUM;

					//波状攻撃パラメータ再作成
					ReCreateCircleParameter();

					InitCircleParameterValue();
					FinishState();
				}
				//エフェクトの色が変更されていたら
				if (change_effect_color)
				{
					change_effect_color = false;
					for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
					{
						circleShotParameter_.effect[i].SetColor(effect_color[0], effect_color[1], effect_color[2]);
					}
				}
				//エフェクトの生存時間変更されていたら
				if (change_effect_limit)
				{
					change_effect_limit = false;
					for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
					{
						circleShotParameter_.effect[i].SetLimit(effect_limit);
					}
				}
				//エフェクトのサイズ変更されていたら
				if (change_effect_size)
				{
					change_effect_size = false;
					for (int  i = 0; i < circleShotParameter_.CUBE_NUM; i++)
					{
						circleShotParameter_.effect[i].SetScale(effect_size);
					}
				}
			}
		}
		//====================================================================================================
		//		Horming
		//====================================================================================================
		{
			static bool change_hormingcube_num = false;		//ホーミングキューブの数を変更したか
			static bool change_horming_effect_color = false;	//ホーミングのエフェクトカラーを変更したか。
			static float horming_effect_color[3];						//エフェクトカラー配列
			static float effect_size = 1.0f;									//エフェクトサイズ
			static int limit_time = 70;										//エフェクト生存時間
			static bool change_limit = false;								//エフェクトの生存時間変更したか
			static bool change_effect_size = false;					//エフェクトのサイズ変更したか
			static bool change_accel = false;							//加速度変更したか
			static float accel = 0.01f;										//加速度
			

			//ホーミングのパラメータ設定
			if (ImGui::TreeNode("HORMING PARAMETER"))
			{
				change_hormingcube_num = ImGui::SliderInt("CUBE NUM", &hormingParameter_.CUBE_NUM, 0, 20);		//キューブ数設定
				ImGui::SliderFloat("FanAngle", &hormingParameter_.fanangle, 60.0f, 180.0f, "%.1f", 1.0f);								//扇の角度設定
				ImGui::DragFloat("InitalVelocity", &hormingParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);								//初期速度設定
				change_accel = ImGui::DragFloat("Acceleration", &accel, 0.001f, 0.0f, 1.0f);													//加速度設定
				ImGui::DragFloat("FanRadius", &hormingParameter_.radius, 0.1f, 1.0f, 10.0f);													//扇の半径
				ImGui::SliderInt("NextShotCoolTime", &hormingParameter_.cooltime, 0, 120);												//次の弾を打つまでのクールタイム
				ImGui::SliderInt("AliveTime", &hormingParameter_.alivetime, 1, 300);																//ホーミング生存時間
				ImGui::DragFloat("HormingAccuracy", &hormingParameter_.horming_accuracy, 0.01f, 0.0f, 1.0f);					//ホーミング精度
				ImGui::DragFloat("SetPositionSpeed", &hormingParameter_.setposition_speed, 0.01f, 1.0f);							//ポジションに向かうスピード
				ImGui::SliderFloat("CUBE SIZE", &hormingParameter_.cubeSize, 0.1f, 1.0f);													//キューブのサイズ
				change_horming_effect_color = ImGui::ColorEdit3("Effect Color", horming_effect_color);								//エフェクトのカラー設定
				change_limit = ImGui::SliderInt("Effect Limit Time", &limit_time, 70, 250);														//エフェクトの生存時間設定
				change_effect_size = ImGui::DragFloat("Effect Size", &effect_size, 0.1f, 0.0f, 20.0f, "%.2f");							//エフェクトのサイズを設定
				ImGui::TreePop();
			}

			if (change_hormingcube_num)
			{
				change_hormingcube_num = false;

				//キューブの数が変更されていたら
				if (hormingParameter_.CUBE_NUM != hormingParameter_.OLD_CUBE_NUM)
				{
					change_horming_effect_color = true;
					change_limit = true;

					//現在のキューブの数を設定
					hormingParameter_.OLD_CUBE_NUM = hormingParameter_.CUBE_NUM;

					//ホーミングパラメータ再作成
					ReCreateHormingParameter();

					InitHormingParameterValue();
					FinishState();
				}
			}
			//Horming Effect Color Change
			if (change_horming_effect_color)
			{
				change_horming_effect_color = false;

				for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
				{
					hormingParameter_.effect[i].SetColor(
						horming_effect_color[0],
						horming_effect_color[1],
						horming_effect_color[2]);
				}
			}
			//生存時間変更されていたら更新
			if (change_limit)
			{
				change_limit = false;
				for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
				{
					hormingParameter_.effect[i].SetLimit(limit_time);
				}
			}
			//エフェクトのサイズ更新されていたら更新
			if(change_effect_size)
			{
				change_effect_size = false;
				for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
				{
					hormingParameter_.effect[i].SetScale(effect_size);
				}
			}
			//加速度変更されていたら
			if (change_accel)
			{
				change_accel = false;
				hormingParameter_.acceleration = accel;
			}

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

		//パラメータ解放
		DestParameter();

		//各パラメータ初期化再設定
		SetIdleParameter(&idleparameter);
		SetRushParameter(&rushparameter);
		SetCircleShotParameter(&circleshotparameter);
		SetHormingParameter(&hormingparameter);

		InitParameter();								//パラメータ情報初期化
		InitCircleParameterValue();			//波状攻撃の情報初期化
		InitHormingParameterValue();		//ホーミングの情報初期化
	}
}

void EnemyHige::InitParameter()
{
	//==========================================================
	//			波状攻撃　初期化
	//==========================================================
	//波状攻撃時のキューブ作成
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];
	//波状攻撃ベクトル作成
	circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];
	//波状攻撃のエフェクト作成
	circleShotParameter_.effect = new AdditionEffect[circleShotParameter_.CUBE_NUM];

	//==========================================================
	//			ホーミング初期化
	//==========================================================
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
	//ホーミングスピード変数作成
	hormingParameter_.speed = new float[hormingParameter_.CUBE_NUM];	
	//エフェクト作成
	hormingParameter_.effect = new AdditionEffect[hormingParameter_.CUBE_NUM];
}

void EnemyHige::ReCreateCircleParameter()
{
	delete[] circleShotParameter_.cube;
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];

	delete[] circleShotParameter_.vec;
	circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];

	delete[] circleShotParameter_.effect;
	circleShotParameter_.effect = new AdditionEffect[circleShotParameter_.CUBE_NUM];
}

void EnemyHige::ReCreateHormingParameter()
{
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

	delete[] hormingParameter_.speed;
	hormingParameter_.speed = new float[hormingParameter_.CUBE_NUM];

	delete[] hormingParameter_.effect;
	hormingParameter_.effect = new AdditionEffect[hormingParameter_.CUBE_NUM];
}

void EnemyHige::InitCircleParameterValue()
{
	//波状攻撃初期化
	for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].SetVisible(false);
		circleShotParameter_.cube[i].SetPosition(0.0f, 0.0f, 0.0f);
		circleShotParameter_.cube[i].SetScale(circleShotParameter_.cubeSize, circleShotParameter_.cubeSize, circleShotParameter_.cubeSize);
	}
}

void EnemyHige::InitHormingParameterValue()
{
	//ホーミング初期化
	for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.cube[i].SetVisible(false);
		hormingParameter_.cube[i].SetPosition(0.0f, 0.0f, 0.0f);
		hormingParameter_.cube[i].SetScale(hormingParameter_.cubeSize, hormingParameter_.cubeSize, hormingParameter_.cubeSize);
		hormingParameter_.shot[i] = false;
		hormingParameter_.cooltimecount[i] = 0;
		hormingParameter_.alivetimecount[i] = 0;
		hormingParameter_.speed[i] = hormingParameter_.inital_velocity;
		hormingParameter_.spawnvec[i].vector = D3DXVECTOR3(0,1.0f,0);
		hormingParameter_.vec[i] = *vector_;
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
	if (circleShotParameter_.effect)
	{
		delete[] circleShotParameter_.effect;
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
	if (hormingParameter_.speed)
	{
		delete[] hormingParameter_.speed;
	}
	if (hormingParameter_.effect)
	{
		delete[] hormingParameter_.effect;
	}
}
