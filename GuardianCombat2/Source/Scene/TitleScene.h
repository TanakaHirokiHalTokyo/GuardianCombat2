#pragma once

//====================================================
//
//		TitleScene
//
//====================================================

#include "Scene.h"

class XModel;
class CEffekseer;
class Texture;
class Sound;
class TitleSelect;

class TitleScene :public Scene
{
public:
	TitleScene();
	~TitleScene();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	void SceneChange();
private:
	XModel* hige_ = nullptr;							//敵のモデル情報
	bool up_ = false;										//上昇フラグ
	float value_ = 0.0f;									//上昇値
	CEffekseer* effect_ = nullptr;					//エフェクト情報
	Sound* bgm_ = nullptr;							//BGM情報
	Sound* selectOKSE_ = nullptr;				//選択時効果音
	TitleSelect* titleSelect_ = nullptr;			//選択管理クラス
};