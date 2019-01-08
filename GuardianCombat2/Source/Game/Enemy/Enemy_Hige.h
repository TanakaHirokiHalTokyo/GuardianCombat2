#pragma once

#include "Enemy.h"
#include <string>

class StatePattern_Enemy;
class Sphere;
class DebugSphere;
class Cube;
class AdditionEffect;
class CEffekseer;
class Texture;
class EnemyHige_Avater;
class OBB;
class EnemyHigeUI;

class ParameterVector
{
public:
	D3DXVECTOR3 vector;
};
namespace EnemyHigeRush				//突進状態のパラメータ
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.5f;						//スピード
		float length = 10.0f;						//突進距離
		float attack = 0.1f;						//ダメージ量(Damage Per Second)
	};
}
namespace EnemyHigeCircleShot					//波状攻撃の情報
{
	struct ENEMY_PARAMETER
	{
		int CUBE_NUM = 10;								//キューブの数
		int OLD_CUBE_NUM = 10;						//前回のキューブの数
		float inital_velocity = 0.1f;						//キューブの飛んでいく初速度
		float acceleration = 0.001f;						//加速度
		float speed = inital_velocity;					//現在のスピード
		float length = 10.0f;									//キューブを飛ばす距離
		float cubeSize = 1.0f;								//キューブのサイズ
		Cube* cube = nullptr;								//キューブ情報
		ParameterVector* vec = nullptr;				//ベクトル情報
		AdditionEffect* effect = nullptr;				//エフェクト情報
	};
}

namespace EnemyHigeHorming									//追跡弾情報
{
	struct ENEMY_PARAMETER
	{
		int CUBE_NUM = 3;											//キューブの数
		int OLD_CUBE_NUM = CUBE_NUM;					//前回のキューブの数
		float inital_velocity = 0.1f;									//キューブの飛んでいく初速度
		float acceleration = 0.001f;									//加速度
		float* speed = nullptr;											//現在のスピード
		float cubeSize = 1.0f;											//キューブのサイズ
		float fanangle = 120.0f;										//キューブスポーン時の扇状角度
		float setposition_speed = 0.1f;							//ポジションに向かうスピード
		float radius = 5.0f;												//扇の半径
		int cooltime = 30;												//次の弾を打つまでのフレーム数
		int* cooltimecount = nullptr;								//クールタイムカウント
		bool* shot = nullptr;											//現在のインデックス番号の弾を打っているのか
		int alivetime = 120;												//生存時間
		int* alivetimecount = nullptr;								//生存時間カウント
		float horming_accuracy = 0.2f;							//ホーミング精度
		Cube* cube = nullptr;											//キューブ情報
		Vector3* vec = nullptr;										//ベクトル情報
		ParameterVector* spawnvec = nullptr;				//スポーン時のベクトル情報
		AdditionEffect* effect = nullptr;							//エフェクト情報
	};
}
namespace EnemyHigeTeleportation						//テレポート情報
{
	struct ENEMY_PARAMETER
	{
		CEffekseer* effect = nullptr;							//テレポートエフェクト
		float distance = 1.0f;										//Playerとの距離
	};
}
namespace EnemyHigeSummons							//分身召喚情報
{
	struct ENEMY_PARAMETER
	{
		int summons_max = 10;																//召喚する最大数
		int summons_num = 6;																//分身召喚する数
		float trans_length = 3.0f;																//分身が移動する距離
		float trans_speed = 0.1f;																//分身が移動するスピード
		EnemyHige_Avater* avater = {};													//分身情報
		float cube_size = 1.0f;																	//キューブの大きさ
		int avater_old_num = summons_num;										//分身の前回までの数
		D3DXVECTOR3 move_position = D3DXVECTOR3(0,0,0);			//移動する場所
		int avater_alive = 0;																		//敵が生きている数
		float cube_speed = 0.1f;																//キューブの飛んでいくスピード
		int attack_interval = 180;															//攻撃の間隔
	};
}
namespace EnemyHigeBurstShot
{
	struct ENEMY_PARAMETER
	{
		
		CEffekseer* effect = nullptr;										//エフェクト
		float effect_size = 1.0f;												//エフェクトのサイズ
		OBB* collision = nullptr;												//コリジョン
		float dps = 20.0f;															//damage per second
		float blur_color[3] = { 255.0f,255.0f,255.0f};				//ブラーのカラー
		int blur_alpha = 240;													//ブラーテクスチャのα値
		bool isblur = true;														//ブラーを使用するのか
	};
}

class EnemyHige :public Enemy
{
public:
	static const float BURSTX;
	static const float BURSTY;
	static const float BURSTZ;

	enum STATE					//さいしょに通常攻撃を記述すること。後ろに特殊攻撃を記述。
	{
		IDLE = 0,						//待機状態
		RUSH,							//突進
		CIRCLESHOT,				//波状攻撃
		HORMING,					//ホーミング弾
		TELEPORTETION,		//テレポート
		BURSTSHOT,				//バーストショット
		SUMMONSAVATER,	//分身召喚
		STATE_MAX,
	};
	const char* StateWord[STATE_MAX]
	{
		"IDLE",
		"RUSH",
		"CIRCLESHOT",
		"HORMING",
		"TELEPORTETION",
		"BURSTSHOT",
		"SUMMONSAVATER",
	};
public:
	EnemyHige();
	~EnemyHige();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	const float DEFAULT_SPEED = 0.1f;			//デフォルトスピード

	STATE GetState();
	void SetState(STATE state);

	//行動を終えた際にIDLE状態に
	void FinishState();

	//突進状態のパラメータ取得・設定
	EnemyHigeRush::ENEMY_PARAMETER GetRushParameter();
	void SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER* parameter);
	void SaveRushParameter(const std::string filename);			//突進パラメータ保存
	void LoadRushParameter(const std::string filename);			//突進パラメータ読込

	//波状攻撃のパラメータ取得・設定
	EnemyHigeCircleShot::ENEMY_PARAMETER GetCircleShotParameter();
	void SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER* parameter);
	void SaveCircleShotParameter(const std::string filename);		//波状攻撃パラメータ保存
	void LoadCircleShotParameter(const std::string filename);	//波状攻撃パラメータ読み込み
	

	//ホーミングのパラメータ取得・設定
	EnemyHigeHorming::ENEMY_PARAMETER GetHormingParameter();
	void SetHormingParameter(EnemyHigeHorming::ENEMY_PARAMETER* parameter);
	void SaveHormingParameter(const std::string filename);		//ホーミングパラメータの保存
	void LoadHormingParameter(const std::string filename);		//ホーミングパラメータ読込

	//テレポートのパラメータ取得・設定
	EnemyHigeTeleportation::ENEMY_PARAMETER GetTeleportParameter();
	void SetTeleportParameter(EnemyHigeTeleportation::ENEMY_PARAMETER* parameter);
	void SaveTeleportParameter(const std::string filename);		//テレポートパラメータ保存
	void LoadTeleportParameter(const std::string filename);		//テレポートパラメータ読込

	//召喚のパラメータ取得・設定
	EnemyHigeSummons::ENEMY_PARAMETER GetSummonsParameter();
	void SetSummonsParameter(EnemyHigeSummons::ENEMY_PARAMETER* parameter);
	void SaveSummonsParameter(const std::string filename);		//召喚パラメータ保存
	void LoadSummonsParameter(const std::string filename);		//召喚パラメータ読込

	//バーストショットのパラメータ取得・設定
	EnemyHigeBurstShot::ENEMY_PARAMETER GetBurstParameter();
	void SetBurstParameter(EnemyHigeBurstShot::ENEMY_PARAMETER* parameter);
	void SaveBurstParameter(const std::string filename);		//バーストパラメータ保存
	void LoadBurstParameter(const std::string filename);		//バーストパラメータ読込

private:
	void DrawDebug();															//Debug表示
	void InitParameter();														//パラメータ初期化
	void ReCreateCircleParameter();									//波状攻撃パラメータ再作成
	void ReCreateHormingParameter();								//ホーミングパラメータ再作成
	void ReCreateSummonsParameter();								//分身召喚パラメータ再作成
	void InitCircleParameterValue();										//波状パラメータ初期化
	void InitHormingParameterValue();								//パラメータの詳細を初期化
	void InitTeleportParameterValue();									//テレポートのパラメータ初期化
	void InitSummonsParameterValue();								//分身召喚パラメータ初期化
	void InitBurstShotParameterValue();								//バーストショットの情報初期化
	void DestParameter();														//パラメータデストラクタ
private:
	EnemyHigeUI* ui_ = nullptr;											//UI
	StatePattern_Enemy* statePattern_ = nullptr;				//ステート状態管理
	STATE state_ = IDLE;														//状態
	Sphere* collision_ = nullptr;											//コリジョン情報
	EnemyHigeRush::ENEMY_PARAMETER rushParameter_ = {};								//突進状態のパラメータ情報
	EnemyHigeCircleShot::ENEMY_PARAMETER circleShotParameter_ = {};			//波状攻撃のパラメータ情報
	EnemyHigeHorming::ENEMY_PARAMETER hormingParameter_ = {};					//ホーミングのパラメータ情報
	EnemyHigeTeleportation::ENEMY_PARAMETER teleportationParameter_ = {};	//テレポートのパラメータ情報
	EnemyHigeSummons::ENEMY_PARAMETER summonsParameter_ = {};				//分身召喚のパラメータ情報
	EnemyHigeBurstShot::ENEMY_PARAMETER burstParameter_ = {};					//バーストショットのパラメータ情報
	XModel* ring_ = nullptr;
};