#pragma once

#include "../Object.h"
class SceneSkinMesh;
class Vector3;

constexpr float PLAYER_DEFAULT_SPEED = 0.005f;
constexpr float PLAYER_DEFAULT_LIFE = 100.0f;

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
protected:
	SceneSkinMesh* animModel_ = nullptr;			//アニメーション付きモデル
	float speed_ = PLAYER_DEFAULT_SPEED;			//プレイヤースピード
	float life_ = PLAYER_DEFAULT_LIFE;				//プレイヤー体力
	Vector3* vector_ = nullptr;						//プレイヤーベクトル
	bool onLand_ = true;							//地面に乗っているか
};