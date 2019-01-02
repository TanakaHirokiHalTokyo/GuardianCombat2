#pragma once
//==========================================================
//
//		敵（ひげ）の分身
//
//==========================================================
#include "Enemy.h"

class StateEnemy;
class EnemyHige;
class Cube;
class StateEnemyAvater;
class Sphere;

class EnemyHige_Avater :public Enemy
{
public:
	EnemyHige_Avater();															//コンストラクタ
	~EnemyHige_Avater();														//デストラクタ
	void Init()override;																//初期化処理
	void Uninit()override;															//終了処理
	void Update()override;														//更新処理
	void BeginDraw()override;													//事前描画処理
	void Draw()override;															//描画処理
	void EndDraw()override;														//終了描画処理

	void SetSpawnVector(D3DXVECTOR3 vec);						//ベクトル設定
	void SetSpawnVector(float x,float y,float z);						//ベクトル設定
	D3DXVECTOR3 GetSpawnVector();									//スポーンベクトルベクトル取得
	void SetMoving(bool flag) { moving_ = flag; }						//移動しているのかを設定
	bool GetMoving() { return moving_; }									//移動しているのかを取得
	Cube* GetCube();																//キューブ取得
	void SetParent(EnemyHige* hige);
	inline EnemyHige* GetParent() { return parent_; }

	void Destroy();
private:
	EnemyHige* parent_ = nullptr;											//親ポインタ
	StateEnemy* rotate_ = nullptr;											//回転制御
	StateEnemyAvater* attack_ = nullptr;								//攻撃制御
	D3DXVECTOR3 spawnVec_ = D3DXVECTOR3(0,0,0);		//スポーンベクトル
	float distance_ = 0.0f;															//進んだ距離
	bool moving_ = true;															//動いているのか
	Cube* cube_ = nullptr;
	Sphere* collision_ = nullptr;
};