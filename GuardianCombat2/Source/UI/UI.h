#pragma once
#include "../Game/Object.h"
#include "../Texture/Texture.h"

class UIState;

class UI :public Object
{
public:
	UI(TextureManager::TextureList type);
	~UI()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	void MoveTexture(float afterSpawnCount,float moveTime,float update_x,float update_y);								//この関数を呼ぶとUpdateでテクスチャが
	void MoveTexture(float afterSpawnCount, float moveTime, D3DXVECTOR3 update_pos);									//この関数を呼ぶとUpdateでテクスチャが動く
	void MoveTexture(float afterSpawnCount, float moveTime, D3DXVECTOR2 update_pos);									//この関数を呼ぶとUpdateでテクスチャが動く

	void ScalingTexture(float afterSpawnCount, float moveTime, float update_x, float update_y);							//この関数を呼ぶとUpdateでテクスチャが
	void ScalingTexture(float afterSpawnCount, float moveTime, D3DXVECTOR3 update_scale);								//この関数を呼ぶとUpdateでテクスチャが動く
	void ScalingTexture(float afterSpawnCount, float moveTime, D3DXVECTOR2 update_scale);								//この関数を呼ぶとUpdateでテクスチャが動く

	Texture* GetTexture();
	//	Move	------------------------------------------------------------------------------------
	float GetCountMove();							//生成開始から何秒で動かすのかを取得
	float GetMoveTime();							//移動開始から何秒間動かすのかを取得
	D3DXVECTOR2 GetAfterPos();						//更新後の座標を取得

	//	Scale	------------------------------------------------------------------------------------
	float GetCountScale();							//生成開始から何秒で動かすのかを取得
	float GetScaleTime();							//移動開始から何秒間動かすのかを取得
	D3DXVECTOR2 GetAfterScale();					//更新後のScaleを取得



private:
	Texture* texture_ = nullptr;
	UIState* stateMove_ = nullptr;					//動きを制御
	UIState* stateScale_ = nullptr;					//サイズを制御
	//		Move Info	----------------------------------------------------------------------------
	float countMove_ = 0;							//生成してから何秒後に移動させるか
	float moveTime_ = 0;							//移動開始から何秒間で移動し終わるのか
	D3DXVECTOR2 updatePos_ = D3DXVECTOR2(0, 0);		//移動後の座標

	//		Scale Info  -----------------------------------------------------------------------------
	float countScale_ = 0;							//生成してから何秒後にサイズを変えるのか
	float scaleTime_ = 0;							//サイズ変更開始から何秒間で変更し終わるのか
	D3DXVECTOR2 updateSize_ = D3DXVECTOR2(1, 1);	//変更後のサイズ
};