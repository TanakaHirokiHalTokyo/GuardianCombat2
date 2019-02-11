#pragma once

#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <d3d9.h>
#include <d3dx9.h>

struct EffectFile
{
	const wchar_t fileName[256];
};


class CEffekseer
{
public:
	struct Transform
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scale;
		D3DXVECTOR3 rotate;
	};

	const enum Effect
	{
		Effect_Bullet,							//弾のエフェクト
		Effect_Teleport,						//テレポート
		Effect_Burst,							//バースト
		Effect_EnemyExplosion,			//敵の爆発エフェクト
		Effect_HitEnemy,						//敵にヒットしたとき
		Effect_Max
	};
	//ファイルパス
	const EffectFile fileName_[Effect_Max]{
		L"resource/Effekseer/simple_bullet.efk",
		L"resource/Effekseer/teleportation.efk",
		L"resource/Effekseer/burstshot.efk",
		L"resource/Effekseer/EnemyExplosion.efk",
		L"resource/Effekseer/Hit.efk",
	};


public:
	CEffekseer(Effect EffectType);					//コンストラクタ　エフェクト名指定
	~CEffekseer();												//デストラクタ
	void Init();													//初期化処理
	void Uninit();												//終了処理
	void Update();												//更新処理
	void Draw();													//描画処理
	
	void SetSpeed(float speed);						//再生スピード設定
	void RepeatEffect(bool repeat);					//リピートするか設定
	void Play();													//再生
	void Paused(bool paused);							//ポーズ設定
	void Stop();													//停止
	bool GetPlaying();										//再生しているか
	void SetVisible(bool flag);							//可視状態設定
	bool GetVisible();										//可視状態取得
	void SetPosition(float x,float y,float z);		//座標設定
	void SetPosition(D3DXVECTOR3 trans);	//座標設定
	D3DXVECTOR3 GetPosition();					//座標取得
	void SetRotate(float x,float y,float z);			//回転設定
	void SetRotate(D3DXVECTOR3 rot);			//回転設定
	D3DXVECTOR3 GetRotate();						//回転取得
	void SetScale(float x, float y,float z);			//スケール値設定
	void SetScale(D3DXVECTOR3 scale);		//スケール値設定
	D3DXVECTOR3 GetScale();						//スケール値取得

private:
	void LoadEffect();										//エフェクト読み込み
	void SetView(D3DXVECTOR3 Pos, D3DXVECTOR3 At, D3DXVECTOR3 Up);								//ビュー行列設定
	void SetProj(float Fov, float ScreenWidth, float ScreenHeight, float Near, float Far);					//プロジェクションセット
	void SetMatrix(D3DXMATRIX mat);																								//Effekseerのワールド変換設定

	Effekseer::Manager*					effekseerManager_;						//エフェクトマネージャー
	EffekseerRendererDX9::Renderer*		effekseerRenderer_;				//エフェクト描画デバイス
	Effekseer::Effect*					effekseerEffect_;									//エフェクトデバイス
	Effekseer::Handle					effekseerHandle_;								//ハンドラー
	CEffekseer::Effect					effectType_;
	bool repeat_ = false;				//繰りかえし再生するか。
	bool paused_ = false;				//ポーズ中か。
	bool playing_ = false;				//再生中か
	bool visible_ = false;				//表示するか
	float speed_ = 1.0f;
	Transform transform_;

};