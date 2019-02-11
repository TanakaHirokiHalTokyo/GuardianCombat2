#pragma once

//========================================================================
//
//		PauseScene
//
//========================================================================

#include "Scene.h"

class Texture;
class PauseSelect;

class PauseScene :public Scene
{
public:
	PauseScene();
	~PauseScene()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	void SetPause(bool flag);
	bool GetPause();

	inline void SetEnableClick(bool flag) { isEnableClick_ = flag; }
private:
	bool isEnableClick_ = true;							//マウスクリックでの編集可能か
	bool pausing_ = false;									//現在ポーズ中か
	PauseSelect* pauseSelect_ = nullptr;		//ポーズ選択制御
	Texture* backImage_ = nullptr;					//裏テクスチャ	
};