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
#include "../../Effekseer/Effekseer.h"
#include "../../Game/MeshField/MeshField.h"
#include "../../DirectXRenderer.h"	
#include "../../Texture/Texture.h"
#include "Enemy_HigeAvater.h"

const D3DXVECTOR3 init_pos = D3DXVECTOR3(-0.1f,0.0f,-8.0f);
const float EnemyHige::BURSTX = 3.5f;
const float EnemyHige::BURSTY = 3.5f;
const float EnemyHige::BURSTZ = 8.5f;

EnemyHige::EnemyHige()
{
	enemyType_ = ENEMY_HIGE;

	normalAttackNum_ = TELEPORTETION;
	specialAttackNum_ = STATE_MAX - (normalAttackNum_ + 1);

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
	enemyCollisions_.emplace_back(collision_);

	ring_ = Object::Create<XModel>();
	ring_->SetModelType(XModel::MODEL_RING);
	ring_->SetScale(0.5f, 0.5f, 0.5f);

	//パラメータ初期化
	InitParameter();

	this->LoadIdleParameter("EnemyHige_Idle");
	this->LoadCircleShotParameter("EnemyHige_CircleShot");
	this->LoadRushParameter("EnemyHige_Rush");
	this->LoadHormingParameter("EnemyHige_Horming");
	this->LoadTeleportParameter("EnemyHige_Teleport");
	this->LoadSummonsParameter("EnemyHige_Summons");
	this->LoadBurstParameter("EnemyHige_BurstShot");
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
	DestParameter();
}

void EnemyHige::Init()
{
	//初期化
	life_ = 100.0f;

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
	vector_->SetUp(0, 1.0f, 0);
	D3DXVECTOR3 right;
	//右ベクトル
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);
	
	//Collision初期化
	collision_->rad = 0.5f;

	FinishState();

	InitCircleParameterValue();
	InitHormingParameterValue();
	InitSummonsParameterValue();
	InitBurstShotParameterValue();
	InitTeleportParameterValue();
}

void EnemyHige::Uninit()
{
}

void EnemyHige::Update()
{
	if (life_ <= 0.0f)
	{
		GameManager::GameOver(true);
		return;
	}

	//敵だけのポーズモード適用するのか判断
	JudgePause();

	if (!enemypause_)
	{
		//ステート制御実行
		statePattern_->Act();

		//範囲内に収める
		float lengthX = fabsf(GetPosition().x);
		float lengthZ = fabsf(GetPosition().z);

		if (lengthX > FIELD_SIZE)
		{
			if (GetPosition().x > 0) { SetPositionX(FIELD_SIZE); }
			else { SetPositionX(-FIELD_SIZE); }
		}
		if (lengthZ > FIELD_SIZE)
		{
			if (GetPosition().z > 0) { SetPositionZ(FIELD_SIZE); }
			else { SetPositionZ(-FIELD_SIZE); }
		}

		//モデル情報更新
		model_->SetRotation(GetRotate());
		model_->SetPosition(GetPosition());
		model_->SetScale(GetScale());

		ring_->SetPosition(GetPosition());
		ring_->SetPositionY(ring_->GetPosition().y + ring_->GetScale().x);
		collision_->pos = GetPosition();
		collision_->pos.y = collision_->pos.y + collision_->rad;

		for (int i = 0; i < summonsParameter_.summons_num; i++)
		{
			summonsParameter_.avater[i].Update();
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

	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].BeginDraw();
	}

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
	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].Draw();
	}


	if (this->GetEditMode())
	{
		//デバッグ表示　ステート・パラメータ表示
		DrawDebug();
	}
	statePattern_->Display();
}

void EnemyHige::EndDraw()
{
	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].EndDraw();
	}

	statePattern_->EndDisplay();
}

EnemyHige::STATE EnemyHige::GetState()
{
	return state_;		//状態取得
}

void EnemyHige::SetState(STATE state)
{
	idleParameter_.idle__counter = 0;
	state_ = state;		//状態設定
	statePattern_->ChangeState();
}
void EnemyHige::FinishState()
{
	enemyBurstCollision_->enable_ = false;
	SetPositionY(MeshField::GetHeight(GetPosition()));
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
//==================================================
//		突進パラメータ保存
//		filename : かき出しファイル名
//==================================================
void EnemyHige::SaveRushParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->rushParameter_.length, sizeof(float));
	file.write((const char*)&this->rushParameter_.speed, sizeof(float));
	file.write((const char*)&this->rushParameter_.attack, sizeof(float));
	file.close();
	
}
//============================================================
//		突進攻撃パラメータ読込
//		filename : 読み込みファイル名（resource/ + .parameter省略）
//=============================================================
void EnemyHige::LoadRushParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。","失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->rushParameter_.length, sizeof(float));
		file.read((char*)&this->rushParameter_.speed, sizeof(float));
		file.read((char*)&this->rushParameter_.attack, sizeof(float));
	}
	file.close();
}
EnemyHigeCircleShot::ENEMY_PARAMETER EnemyHige::GetCircleShotParameter()
{
	return circleShotParameter_;		//波状攻撃取得
}
void EnemyHige::SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER* parameter)
{
	circleShotParameter_ = *parameter;	//波状攻撃設定
}
//=====================================================================
//		波状攻撃パラメータ保存
//		filename : かき出しファイル名
//======================================================================
void EnemyHige::SaveCircleShotParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->circleShotParameter_.acceleration,sizeof(circleShotParameter_.acceleration));
	file.write((const char*)&this->circleShotParameter_.cubeSize,sizeof(circleShotParameter_.cubeSize));
	file.write((const char*)&this->circleShotParameter_.CUBE_NUM,sizeof(circleShotParameter_.CUBE_NUM));
	file.write((const char*)&this->circleShotParameter_.inital_velocity,sizeof(circleShotParameter_.inital_velocity));
	file.write((const char*)&this->circleShotParameter_.length,sizeof(circleShotParameter_.length));
	file.close();

	for (int i = 0; i < this->circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].SaveParameter(filename + "_cube");
		circleShotParameter_.effect[i].SaveParameter(filename + "_effect");
	}
	
}
//========================================================================
//		波状攻撃パラメータ読み込み
//		filename : 読み込みファイル名
//========================================================================
void EnemyHige::LoadCircleShotParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->circleShotParameter_.acceleration, sizeof(circleShotParameter_.acceleration));
		file.read((char*)&this->circleShotParameter_.cubeSize, sizeof(circleShotParameter_.cubeSize));
		file.read((char*)&this->circleShotParameter_.CUBE_NUM, sizeof(circleShotParameter_.CUBE_NUM));
		file.read((char*)&this->circleShotParameter_.inital_velocity, sizeof(circleShotParameter_.inital_velocity));
		file.read((char*)&this->circleShotParameter_.length, sizeof(circleShotParameter_.length));
	}
	file.close();

	this->ReCreateCircleParameter();
	
	for (int i = 0; i < this->circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].LoadParameter(filename + "_cube");
		circleShotParameter_.cube[i].SetScale(circleShotParameter_.cubeSize, circleShotParameter_.cubeSize, circleShotParameter_.cubeSize);
		circleShotParameter_.effect[i].LoadParameter(filename + "_effect");
	}
}
EnemyHigeHorming::ENEMY_PARAMETER EnemyHige::GetHormingParameter()
{
	return hormingParameter_;			//ホーミングパラメータ取得
}
void EnemyHige::SetHormingParameter(EnemyHigeHorming::ENEMY_PARAMETER * parameter)
{
	hormingParameter_ = *parameter;		//ホーミングパラメータ設定
}
//========================================================================
//		Hormingパラメータ保存
//		filename : 書き込みファイル名
//========================================================================
void EnemyHige::SaveHormingParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->hormingParameter_.acceleration,sizeof(hormingParameter_.acceleration));
	file.write((const char*)&this->hormingParameter_.alivetime,sizeof(hormingParameter_.alivetime));
	file.write((const char*)&this->hormingParameter_.cooltime,sizeof(hormingParameter_.cooltime));
	file.write((const char*)&this->hormingParameter_.cubeSize,sizeof(hormingParameter_.cubeSize));
	file.write((const char*)&this->hormingParameter_.CUBE_NUM,sizeof(hormingParameter_.CUBE_NUM));
	file.write((const char*)&this->hormingParameter_.fanangle, sizeof(hormingParameter_.fanangle));
	file.write((const char*)&this->hormingParameter_.horming_accuracy,sizeof(hormingParameter_.horming_accuracy));
	file.write((const char*)&this->hormingParameter_.inital_velocity,sizeof(hormingParameter_.inital_velocity));
	file.write((const char*)&this->hormingParameter_.radius,sizeof(hormingParameter_.radius));
	file.write((const char*)&this->hormingParameter_.setposition_speed,sizeof(hormingParameter_.setposition_speed));
	file.close();

	for (int i = 0; i < this->hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.cube[i].SaveParameter(filename + "_cube");
		hormingParameter_.effect[i].SaveParameter(filename + "_effect");
	}
}
//================================================================
//			ホーミングパラメータ読み込み
//			filename : 読み込みファイル名
//================================================================
void EnemyHige::LoadHormingParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->hormingParameter_.acceleration, sizeof(hormingParameter_.acceleration));
		file.read((char*)&this->hormingParameter_.alivetime, sizeof(hormingParameter_.alivetime));
		file.read((char*)&this->hormingParameter_.cooltime, sizeof(hormingParameter_.cooltime));
		file.read((char*)&this->hormingParameter_.cubeSize, sizeof(hormingParameter_.cubeSize));
		file.read((char*)&this->hormingParameter_.CUBE_NUM, sizeof(hormingParameter_.CUBE_NUM));
		file.read((char*)&this->hormingParameter_.fanangle, sizeof(hormingParameter_.fanangle));
		file.read((char*)&this->hormingParameter_.horming_accuracy, sizeof(hormingParameter_.horming_accuracy));
		file.read((char*)&this->hormingParameter_.inital_velocity, sizeof(hormingParameter_.inital_velocity));
		file.read((char*)&this->hormingParameter_.radius, sizeof(hormingParameter_.radius));
		file.read((char*)&this->hormingParameter_.setposition_speed, sizeof(hormingParameter_.setposition_speed));
	}
	file.close();

	this->ReCreateHormingParameter();
	for (int i = 0; i < this->hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.cube[i].LoadParameter(filename + "_cube");
		hormingParameter_.cube[i].SetScale(hormingParameter_.cubeSize, hormingParameter_.cubeSize, hormingParameter_.cubeSize);
		hormingParameter_.effect[i].LoadParameter(filename + "_effect");
	}
}
EnemyHigeTeleportation::ENEMY_PARAMETER EnemyHige::GetTeleportParameter()
{
	return teleportationParameter_;
}
void EnemyHige::SetTeleportParameter(EnemyHigeTeleportation::ENEMY_PARAMETER * parameter)
{
	teleportationParameter_ = *parameter;
}
//=====================================================================
//		テレポートパラメータ保存
//		filename : 書き込みファイル名
//=====================================================================
void EnemyHige::SaveTeleportParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->teleportationParameter_.distance,sizeof(teleportationParameter_.distance));
	file.close();
}
//====================================================================
//		テレポートパラメータ読込
//		filename : 読み込みファイル名	s5e201938
//=====================================================================
void EnemyHige::LoadTeleportParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->teleportationParameter_.distance, sizeof(teleportationParameter_.distance));
	}
	file.close();
}
EnemyHigeSummons::ENEMY_PARAMETER EnemyHige::GetSummonsParameter()
{
	return summonsParameter_;
}
void EnemyHige::SetSummonsParameter(EnemyHigeSummons::ENEMY_PARAMETER * parameter)
{
	summonsParameter_ = *parameter;
}
//==========================================================================
//			分身召喚パラメータ保存
//			filename : 書き込みファイル名
//==========================================================================
void EnemyHige::SaveSummonsParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->summonsParameter_.attack_interval,sizeof(summonsParameter_.attack_interval));
	file.write((const char*)&this->summonsParameter_.cube_size,sizeof(summonsParameter_.cube_size));
	file.write((const char*)&this->summonsParameter_.cube_speed,sizeof(summonsParameter_.cube_speed));
	file.write((const char*)&this->summonsParameter_.summons_max,sizeof(summonsParameter_.summons_max));
	file.write((const char*)&this->summonsParameter_.summons_num,sizeof(summonsParameter_.summons_num));
	file.write((const char*)&this->summonsParameter_.trans_length,sizeof(summonsParameter_.trans_length));
	file.write((const char*)&this->summonsParameter_.trans_speed,sizeof(summonsParameter_.trans_speed));
	file.write((const char*)&this->moveSpeedToPoint_,sizeof(moveSpeedToPoint_));
	file.close();

	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].SetVisible(false);
		summonsParameter_.avater[i].GetCube()->SaveParameter(filename + "_cube");
	}
}
//==========================================================================
//			分身召喚パラメータ読込
//			filename : 読み込みファイル名
//==========================================================================
void EnemyHige::LoadSummonsParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->summonsParameter_.attack_interval, sizeof(summonsParameter_.attack_interval));
		file.read((char*)&this->summonsParameter_.cube_size, sizeof(summonsParameter_.cube_size));
		file.read((char*)&this->summonsParameter_.cube_speed, sizeof(summonsParameter_.cube_speed));
		file.read((char*)&this->summonsParameter_.summons_max, sizeof(summonsParameter_.summons_max));
		file.read((char*)&this->summonsParameter_.summons_num, sizeof(summonsParameter_.summons_num));
		file.read((char*)&this->summonsParameter_.trans_length, sizeof(summonsParameter_.trans_length));
		file.read((char*)&this->summonsParameter_.trans_speed, sizeof(summonsParameter_.trans_speed));
		file.read((char*)&this->moveSpeedToPoint_, sizeof(moveSpeedToPoint_));
	}

	file.close();

	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].GetCube()->LoadParameter(filename + "_cube");
	}
}
EnemyHigeBurstShot::ENEMY_PARAMETER EnemyHige::GetBurstParameter()
{
	return burstParameter_;
}
void EnemyHige::SetBurstParameter(EnemyHigeBurstShot::ENEMY_PARAMETER* parameter)
{
	burstParameter_ = *parameter;
}
//==============================================================================
//			バーストパラメータ保存
//			filename : 書き込みファイル名
//==============================================================================
void EnemyHige::SaveBurstParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->burstParameter_.dps,sizeof(burstParameter_.dps));
	file.write((const char*)&this->burstParameter_.effect_size, sizeof(burstParameter_.effect_size));
	file.write((const char*)&this->burstParameter_.blur_color[0], sizeof(burstParameter_.blur_color[0]));
	file.write((const char*)&this->burstParameter_.blur_color[1], sizeof(burstParameter_.blur_color[1]));
	file.write((const char*)&this->burstParameter_.blur_color[2], sizeof(burstParameter_.blur_color[2]));
	file.write((const char*)&this->burstParameter_.blur_alpha, sizeof(burstParameter_.blur_alpha));
	file.write((const char*)&this->burstParameter_.isblur, sizeof(burstParameter_.isblur));
	file.close();
}
//==============================================================================
//			バーストパラメータ読込
//			filename : 読み込みファイル名
//==============================================================================
void EnemyHige::LoadBurstParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->burstParameter_.dps, sizeof(burstParameter_.dps));
		file.read((char*)&this->burstParameter_.effect_size, sizeof(burstParameter_.effect_size));
		file.read((char*)&this->burstParameter_.blur_color[0], sizeof(burstParameter_.blur_color[0]));
		file.read((char*)&this->burstParameter_.blur_color[1], sizeof(burstParameter_.blur_color[1]));
		file.read((char*)&this->burstParameter_.blur_color[2], sizeof(burstParameter_.blur_color[2]));
		file.read((char*)&this->burstParameter_.blur_alpha, sizeof(burstParameter_.blur_alpha));
		file.read((char*)&this->burstParameter_.isblur, sizeof(burstParameter_.isblur));

		burstParameter_.effect->SetScale(burstParameter_.effect_size, burstParameter_.effect_size, burstParameter_.effect_size);
	}
	file.close();
}
void EnemyHige::DrawDebug()
{
	static int listbox_item_current = 1;			//リストボックスの初期選択番号
	static bool changeState = false;				//状態を変更したか
	static bool reset_position = false;			//敵のポジションリセットフラグ
	static bool reset_parameter = false;		//パラメータリセットフラグ

	ImGui::SetNextWindowPos(ImVec2(0.0f,0.0f));
	ImGui::SetNextWindowSize(ImVec2((float)ScreenWidth/ 3.0f,(float)ScreenHeight));
	//Imgui 表示
	ImGui::Begin(u8"敵のパラメータ情報");
	ImGui::Text(u8"F2キー : 敵ポーズ");
	ImGui::Text(u8"座標 %f %f %f",GetPosition().x, GetPosition().y, GetPosition().z);
	ImGui::Text(u8"前方向ベクトル\t %f %f %f",vector_->GetFront().x, vector_->GetFront().y, vector_->GetFront().z);
	ImGui::Text(u8"右方向ベクトル\t %f %f %f", vector_->GetRight().x, vector_->GetRight().y, vector_->GetRight().z);
	ImGui::Text(u8"上方向ベクトル\t %f %f %f", vector_->GetUp().x, vector_->GetUp().y, vector_->GetUp().z);
	ImGui::Text(u8"敵の分身の生きている数 : %d",summonsParameter_.avater_alive);
	reset_position = ImGui::Button(u8"敵の座標初期化");
	reset_parameter = ImGui::Button(u8"パラメータ情報初期化");
	if (ImGui::Button(u8"パラメータ保存"))
	{
		this->SaveIdleParameter("EnemyHige_Idle");
		this->SaveRushParameter("EnemyHige_Rush");
		this->SaveCircleShotParameter("EnemyHige_CircleShot");
		this->SaveHormingParameter("EnemyHige_Horming");
		this->SaveTeleportParameter("EnemyHige_Teleport");
		this->SaveSummonsParameter("EnemyHige_Summons");
		this->SaveBurstParameter("EnemyHige_BurstShot");
	}
	ImGui::Checkbox(u8"オートアタック",&autoAttack_);
	ImGui::Checkbox(u8"無敵", &invincible_);											//無敵
	ImGui::SliderFloat(u8"体力",&life_,0.0f,ENEMY_MAX_LIFE);
	ImGui::Text(u8"攻撃パターン : %s",StateWord[state_]);

	//次に設定するSTATE
	changeState = ImGui::ListBox(u8"攻撃パターン指定（クリック）", &listbox_item_current, StateWord, IM_ARRAYSIZE(StateWord), 4);
	if (ImGui::CollapsingHeader(u8"状態変化HPの割合パラメータ"))
	{
		//敵の状態変化させるHPの割合設定
		std::array<float, 4> value;

		value = idleParameter_.hp_ratio_;
		if (ImGui::DragFloat(u8"1段階目 : HP割合", &value[0],0.01f,0.0f,1.0f))
		{
			if (value[0] > idleParameter_.hp_ratio_[1] && value[0] <= 1.0f) { idleParameter_.hp_ratio_[0] = value[0]; }
		}
		if (ImGui::DragFloat(u8"2段階目 : HP割合", &value[1], 0.01f, 0.01f, 1.0f))
		{
			if (value[1] > idleParameter_.hp_ratio_[2] && value[1] < idleParameter_.hp_ratio_[0]) { idleParameter_.hp_ratio_[1] = value[1]; }
		}
		if (ImGui::DragFloat(u8"3段階目 : HP割合", &value[2], 0.01f, 0.01f, 1.0f))
		{
			if (value[2] > idleParameter_.hp_ratio_[3] && value[2] < idleParameter_.hp_ratio_[1]) { idleParameter_.hp_ratio_[2] = value[2]; }
		}
		if (ImGui::DragFloat(u8"4段階目 : HP割合", &value[3], 0.01f, 0.01f, 1.0f))
		{
			if (value[3] < idleParameter_.hp_ratio_[2] && value[3] >= 0.0f) { idleParameter_.hp_ratio_[3] = value[3]; }
		}
	}
	if (ImGui::CollapsingHeader(u8"通常攻撃確率パラメータ"))
	{
		std::array<double, 5> normal = idleParameter_.normalAttackLuck;
		if (ImGui::InputDouble(u8"1段階目 : 通常攻撃確率", &normal[0]))
		{
			if (normal[0] >= 0.0 && normal[0] <= 1.0) { idleParameter_.normalAttackLuck[0] = normal[0]; }
		}
		if (ImGui::InputDouble(u8"2段階目 : 通常攻撃確率", &normal[1]))
		{
			if (normal[1] >= 0.0 && normal[1] <= 1.0) { idleParameter_.normalAttackLuck[1] = normal[1]; }
		}
		if (ImGui::InputDouble(u8"3段階目 : 通常攻撃確率", &normal[2]))
		{
			if (normal[2] >= 0.0 && normal[2] <= 1.0) { idleParameter_.normalAttackLuck[2] = normal[2]; }
		}
		if (ImGui::InputDouble(u8"4段階目 : 通常攻撃確率", &normal[3]))
		{
			if (normal[3] >= 0.0 && normal[3] <= 1.0) { idleParameter_.normalAttackLuck[3] = normal[3]; }
		}
		if (ImGui::InputDouble(u8"5段階目 : 通常攻撃確率", &normal[4]))
		{
			if (normal[4] >= 0.0 && normal[4] <= 1.0) { idleParameter_.normalAttackLuck[4] = normal[4]; }
		}
		for (size_t i = 0; i < idleParameter_.specialAttackLuck.size(); i++)
		{
			idleParameter_.specialAttackLuck[i] = 1.0 - idleParameter_.normalAttackLuck[i];
		}
	}
	
	if (ImGui::CollapsingHeader(u8"待機状態詳細"))
	{
		//待機状態のパラメータ設定
		if (ImGui::TreeNode(u8"待機状態パラメータ"))
		{
			ImGui::DragFloat(u8"プレイヤーにこの距離まで近づく", &idleParameter_.approache_length, 0.1f, 0.0f, 30.0f);
			ImGui::DragFloat(u8"スピード", &idleParameter_.speed, 0.001f, 0.0f, 0.1f);			//スピード設定
			ImGui::DragFloat(u8"振り向き速度", &idleParameter_.rot_angle, 0.01f, 0.01f, 6.0f);		//回転スピード設定
			std::array<int, 5> count;
			count = idleParameter_.count;
			if (ImGui::DragInt(u8"1段階目 : 待機時間", &count[0],1,0,300))
			{
				if (count[0] >= 0) { idleParameter_.count[0] = count[0]; }
			}
			if (ImGui::DragInt(u8"2段階目 : 待機時間", &count[1], 1, 0, 300))
			{
				if (count[1] >= 0) { idleParameter_.count[1] = count[1]; }
			}
			if (ImGui::DragInt(u8"3段階目 : 待機時間", &count[2], 1, 0, 300))
			{
				if (count[2] >= 0) { idleParameter_.count[2] = count[2]; }
			}
			if (ImGui::DragInt(u8"4段階目 : 待機時間", &count[3], 1, 0, 300))
			{
				if (count[3] >= 0) { idleParameter_.count[3] = count[3]; }
			}
			if (ImGui::DragInt(u8"5段階目 : 待機時間", &count[4], 1, 0, 300))
			{
				if (count[4] >= 0) { idleParameter_.count[4] = count[4]; }
			}
			
			ImGui::TreePop();
		}
	}
	//通常攻撃パラメータ設定
	if (ImGui::CollapsingHeader(u8"通常攻撃詳細"))
	{
		//==========================================================================================================
		//		Rush
		//=========================================================================================================
		//突進状態のパラメータ設定
		if (ImGui::TreeNode(u8"突進攻撃(Rush)パラメータ"))
		{
			ImGui::DragFloat(u8"スピード", &rushParameter_.speed, 0.01f, 0.0f, 1.0f);			//スピード設定
			ImGui::DragFloat(u8"突進する距離", &rushParameter_.length, 1.0f, 0.0f, 100.0f);			//突進距離設定
			ImGui::DragFloat(u8"1秒間当たりのダメージ量",&rushParameter_.attack,0.1f,0.0f,30.0f);
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

			static bool init = false;
			if (!init)
			{
				init = true;
				effect_color[0] = circleShotParameter_.effect[0].GetColor().r;
				effect_color[1] = circleShotParameter_.effect[0].GetColor().g;
				effect_color[2] = circleShotParameter_.effect[0].GetColor().b;

				effect_limit = circleShotParameter_.effect[0].GetLimit();
				effect_size = circleShotParameter_.effect[0].GetScale();
			}
			

			//波状攻撃のパラメータ設定
			if (ImGui::TreeNode(u8"波状攻撃(CIRCLESHOT)パラメータ"))
			{
				change_cube_num = ImGui::SliderInt(u8"弾の数", &circleShotParameter_.CUBE_NUM, 1, 20);				//キューブ数設定
				ImGui::DragFloat(u8"初速度", &circleShotParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);						//初期速度設定
				ImGui::DragFloat(u8"加速度", &circleShotParameter_.acceleration, 0.001f, 0.0f, 1.0f);							//加速度設定
				ImGui::DragFloat(u8"飛ばす距離", &circleShotParameter_.length, 1.0f, 0.0f, 100.0f);												//キューブを飛ばす距離設定
				ImGui::SliderFloat(u8"弾の大きさ", &circleShotParameter_.cubeSize, 0.1f, 1.0f);											//キューブのサイズ
				change_effect_color = ImGui::ColorEdit3(u8"エフェクト色",effect_color);															//エフェクトのカラー設定
				change_effect_size = ImGui::DragFloat(u8"エフェクトサイズ",&effect_size,0.1f,0.0f,10.0f);											//エフェクトサイズの設定
				change_effect_limit = ImGui::SliderInt(u8"エフェクト生存時間",&effect_limit,0,150);													//エフェクトの生存時間設定

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
				for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
				{
					circleShotParameter_.effect[i].SetScale(effect_size);
				}
			}
		}
		//====================================================================================================
		//		Horming
		//====================================================================================================
		{
			static bool change_hormingcube_num = false;																//ホーミングキューブの数を変更したか
			static bool change_horming_effect_color = false;															//ホーミングのエフェクトカラーを変更したか。
			static float horming_effect_color[3];																				//エフェクトカラー配列
			static float effect_size = 1.0f;																							//エフェクトサイズ
			static int limit_time = hormingParameter_.effect[0].GetLimit();										//エフェクト生存時間
			static bool change_limit = false;																						//エフェクトの生存時間変更したか
			static bool change_effect_size = false;																			//エフェクトのサイズ変更したか
			static bool change_accel = false;																					//加速度変更したか
			static float accel = hormingParameter_.acceleration;													//加速度

			static bool init = false;
			if (!init)
			{
				init = true;
				horming_effect_color[0] = hormingParameter_.effect[0].GetColor().r;
				horming_effect_color[1] = hormingParameter_.effect[0].GetColor().g;
				horming_effect_color[2] = hormingParameter_.effect[0].GetColor().b;

				effect_size = hormingParameter_.effect[0].GetScale();
			}

			//ホーミングのパラメータ設定
			if (ImGui::TreeNode(u8"ホーミング(HORMING)パラメータ"))
			{
				change_hormingcube_num = ImGui::SliderInt(u8"弾の数", &hormingParameter_.CUBE_NUM, 1, 20);						//キューブ数設定
				ImGui::SliderFloat(u8"展開角度", &hormingParameter_.fanangle, 60.0f, 180.0f, "%.1f", 1.0f);										//扇の角度設定
				ImGui::DragFloat(u8"初速度", &hormingParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);													//初期速度設定
				change_accel = ImGui::DragFloat(u8"加速度", &accel, 0.001f, 0.0f, 1.0f);																		//加速度設定
				ImGui::DragFloat(u8"弾の座標上昇値", &hormingParameter_.radius, 0.1f, 1.0f, 10.0f);													//扇の半径
				ImGui::SliderInt(u8"次の弾を打つまでの時間", &hormingParameter_.cooltime, 0, 120);												//次の弾を打つまでのクールタイム
				ImGui::SliderInt(u8"弾の生存時間", &hormingParameter_.alivetime, 1, 300);																//ホーミング生存時間
				ImGui::DragFloat(u8"ホーミング精度", &hormingParameter_.horming_accuracy, 0.01f, 0.0f, 1.0f);								//ホーミング精度
				ImGui::DragFloat(u8"弾の座標上昇速度", &hormingParameter_.setposition_speed, 0.01f, 1.0f);									//ポジションに向かうスピード
				ImGui::SliderFloat(u8"弾の速度", &hormingParameter_.cubeSize, 0.1f, 1.0f);																//キューブのサイズ
				change_horming_effect_color = ImGui::ColorEdit3(u8"エフェクト色", horming_effect_color);										//エフェクトのカラー設定
				change_limit = ImGui::SliderInt(u8"エフェクト生存時間", &limit_time, 70, 250);															//エフェクトの生存時間設定
				change_effect_size = ImGui::DragFloat(u8"エフェクトサイズ", &effect_size, 0.1f, 0.0f, 20.0f, "%.2f");						//エフェクトのサイズを設定
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
		//====================================================================================================
		//		Teleportation
		//====================================================================================================
		{
			if (ImGui::TreeNode(u8"テレポート(Teleport)パラメータ"))
			{
				ImGui::DragFloat(u8"テレポート後のプレイヤーとの距離",&teleportationParameter_.distance,0.1f,0.0f,20.0f);		//プレイヤーとの距離を設定
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader(u8"特殊攻撃詳細"))
	{
		//=====================================================================================================
		//			SummonsAvater
		//=====================================================================================================
		{
			static bool change_summon_num = false;

			if (ImGui::TreeNode(u8"分身召喚(SUMMONAVATER)パラメータ"))
			{
				if (ImGui::Button(u8"すべての分身消去"))
				{
					for (int i = 0; i < summonsParameter_.summons_max; i++)
					{
						summonsParameter_.avater_alive = 0;
						summonsParameter_.avater[i].Init();
						summonsParameter_.avater[i].SetVisible(false);
						summonsParameter_.avater[i].GetCube()->GetCollision()->enable_ = false;
					}
				}
				change_summon_num = ImGui::SliderInt(u8"召喚する分身の数", &summonsParameter_.summons_num, 0, summonsParameter_.summons_max);
				ImGui::DragFloat(u8"分身が移動する距離", &summonsParameter_.trans_length, 0.01f, 0.0f, 7.0f);
				ImGui::DragFloat(u8"分身が移動するスピード", &moveSpeedToPoint_, 0.01f, 0.01f, summonsParameter_.trans_length);
				ImGui::SliderFloat(u8"弾のサイズ",&summonsParameter_.cube_size,0.0f,3.0f);
				ImGui::SliderFloat(u8"弾のスピード", &summonsParameter_.cube_speed, 0.01f, 1.0f);
				ImGui::SliderInt(u8"次の攻撃が来るまでの時間",&summonsParameter_.attack_interval,10,240);
				ImGui::TreePop();
			}
			if (change_summon_num)
			{
				change_summon_num = false;
				if (summonsParameter_.summons_num != summonsParameter_.avater_old_num)
				{
					summonsParameter_.avater_old_num = summonsParameter_.summons_num;
					ReCreateSummonsParameter();
					InitSummonsParameterValue();
					FinishState();
				}
			}
		}
		//=====================================================================================================
		//			BurstShot
		//=====================================================================================================
		{
			if (ImGui::TreeNode(u8"バースト(BURSTSHOT)パラメータ"))
			{
				ImGui::Checkbox(u8"ブラーを使用するか",&burstParameter_.isblur);
				ImGui::DragFloat(u8"バーストの大きさ", &burstParameter_.effect_size, 0.01f, 0.1f, 3.0f);
				ImGui::DragFloat(u8"1秒間に与えるダメージ", &burstParameter_.dps, 0.1f, 0.0f, 100.0f);
				ImGui::ColorEdit3(u8"ブラーテクスチャの色",burstParameter_.blur_color);
				ImGui::DragInt(u8"ブラーテクスチャのアルファ値",&burstParameter_.blur_alpha,1);
				ImGui::TreePop();
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
		EnemyHigeSummons::ENEMY_PARAMETER summonsparameter;

		//パラメータ解放
		DestParameter();

		//各パラメータ初期化再設定
		SetIdleParameter(&idleparameter);
		SetRushParameter(&rushparameter);
		SetCircleShotParameter(&circleshotparameter);
		SetHormingParameter(&hormingparameter);
		SetSummonsParameter(&summonsparameter);

		InitParameter();								//パラメータ情報初期化
		InitCircleParameterValue();			//波状攻撃の情報初期化
		InitHormingParameterValue();		//ホーミングの情報初期化
		InitSummonsParameterValue();
	}
}

void EnemyHige::InitParameter()
{
	for (size_t i = 0; i < idleParameter_.specialAttackLuck.size(); i++)
	{
		idleParameter_.specialAttackLuck[i] = 1.0 - idleParameter_.normalAttackLuck[i];
	}

	//==========================================================
	//			波状攻撃　初期化
	//==========================================================
	//波状攻撃時のキューブ作成
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];
	//波状攻撃ベクトル作成
	circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];
	//波状攻撃のエフェクト作成
	circleShotParameter_.effect = new AdditionEffect[circleShotParameter_.CUBE_NUM];

	InitCircleParameterValue();

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

	InitHormingParameterValue();

	//=========================================================
	//			テレポート初期化
	//=========================================================
	teleportationParameter_.effect = new CEffekseer(CEffekseer::Effect_Teleport);
	InitTeleportParameterValue();

	//=========================================================
	//			分身召喚の初期化
	//=========================================================
	summonsParameter_.avater = new EnemyHige_Avater[summonsParameter_.summons_max];
	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].SetParent(this);
		summonsParameter_.avater[i].Init();
		summonsParameter_.avater[i].SetVisible(false);
		summonsParameter_.trans_length = 3.0f;
		summonsParameter_.trans_speed = 0.1f;

	}
	InitSummonsParameterValue();
	//=========================================================
	//			バーストショット初期化
	//=========================================================
	burstParameter_.effect = new CEffekseer(CEffekseer::Effect_Burst);
	burstParameter_.collision = new OBB();
	enemyBurstCollision_ = burstParameter_.collision;
	InitBurstShotParameterValue();
}
void EnemyHige::DestParameter()
{
	if (circleShotParameter_.cube)
	{
		SAFE_DELETE_ARRAY(circleShotParameter_.cube);
	}
	if (circleShotParameter_.vec)
	{
		SAFE_DELETE_ARRAY(circleShotParameter_.vec);
	}
	if (circleShotParameter_.effect)
	{
		SAFE_DELETE_ARRAY(circleShotParameter_.effect);
	}
	if (hormingParameter_.cube)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.cube);
	}
	if (hormingParameter_.vec)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.vec);
	}
	if (hormingParameter_.spawnvec)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.spawnvec);
	}
	if (hormingParameter_.shot)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.shot);
	}
	if (hormingParameter_.cooltimecount)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.cooltimecount);
	}
	if (hormingParameter_.alivetimecount)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.alivetimecount);
	}
	if (hormingParameter_.speed)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.speed);
	}
	if (hormingParameter_.effect)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.effect);
	}
	if (teleportationParameter_.effect)
	{
		SAFE_DELETE(teleportationParameter_.effect);
	}
	if (summonsParameter_.avater)
	{
		SAFE_DELETE_ARRAY(summonsParameter_.avater);
	}
	if (burstParameter_.effect)
	{
		burstParameter_.effect->Uninit();
		SAFE_DELETE(burstParameter_.effect);
	}
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

void EnemyHige::ReCreateSummonsParameter()
{
	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].Init();
		summonsParameter_.avater[i].SetVisible(false);
	}
}

void EnemyHige::InitCircleParameterValue()
{
	//波状攻撃初期化
	for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.effect[i].SetVisible(false);
		circleShotParameter_.cube[i].Init();
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
		hormingParameter_.effect[i].SetVisible(false);
		hormingParameter_.cube[i].Init();
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
void EnemyHige::InitTeleportParameterValue()
{
	teleportationParameter_.distance = 1.0f;
	teleportationParameter_.effect->Init();
	teleportationParameter_.effect->RepeatEffect(false);
	teleportationParameter_.effect->SetVisible(false);
	teleportationParameter_.effect->SetPosition(GetPosition());
	teleportationParameter_.effect->SetScale(1.0f, 1.0f, 1.0f);
}
void EnemyHige::InitSummonsParameterValue()
{
	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].Init();
		summonsParameter_.avater[i].SetVisible(false);
		summonsParameter_.trans_length = 3.0f;
		summonsParameter_.trans_speed = 0.1f;
	}
}
void EnemyHige::InitBurstShotParameterValue()
{
	burstParameter_.effect->Init();
	burstParameter_.effect->SetVisible(false);
	burstParameter_.collision->enable_ = false;
	burstParameter_.collision->m_fLength[0] = BURSTX;
	burstParameter_.collision->m_fLength[1] = BURSTY;
	burstParameter_.collision->m_fLength[2] = BURSTZ;
	burstParameter_.collision->m_NormaDirect[0] = D3DXVECTOR3(1.0f,0.0f,0.0f);
	burstParameter_.collision->m_NormaDirect[1] = D3DXVECTOR3(0.0f,1.0f,0.0f);
	burstParameter_.collision->m_NormaDirect[2] = D3DXVECTOR3(0.0f,0.0f,1.0f);
	burstParameter_.collision->object_ = this;
	burstParameter_.collision->m_Pos = GetPosition();
}