#pragma once

#include "../Object.h"
class SceneSkinMesh;
class Vector3;
class Camera;
class PlayerLifeBar;

const float PLAYER_DEFAULT_SPEED = 0.005f;
const float PLAYER_DEFAULT_LIFE = 100.0f;

class Player :public Object
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	SceneSkinMesh* GetAnimModel() {
		if (animModel_ != nullptr)
		{
			return animModel_;
		}
		return nullptr;
	}
	Vector3* GetVector();
	void SetOnLand(bool flag);
	bool GetOnLand();
	void SetCamera(Camera* camera);						//PlayerCameraを設定
	Camera* GetCamera();											//PlayerCameraを取得
	float GetLife();															//Life取得
	void DecreaseLife(float value);								//Life減算

	inline bool GetInvincible() { return invincible_; }
protected:
	bool invincible_ = false;											//無敵
	Camera* camera_ = nullptr;									//カメラ情報
	SceneSkinMesh* animModel_ = nullptr;				//アニメーション付きモデル
	float speed_ = PLAYER_DEFAULT_SPEED;			//プレイヤースピード
	float life_ = PLAYER_DEFAULT_LIFE;					//プレイヤー体力
	PlayerLifeBar* lifeBar_ = nullptr;							//ライフバーポインタ
	Vector3* vector_ = nullptr;										//プレイヤーベクトル
	bool onLand_ = true;												//地面に乗っているか
};