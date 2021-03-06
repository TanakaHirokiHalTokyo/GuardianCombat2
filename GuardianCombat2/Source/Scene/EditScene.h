#pragma once

#include "Scene.h"

class Player;
class PauseScene;
class Sound;

class EditScene :public Scene
{
public:
	EditScene();
	~EditScene()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

private:
	Player* player_ = nullptr;							//プレイヤー情報
	PauseScene* pauseScene_ = nullptr;		//ポーズシーン情報
	Sound* bgm_ = nullptr;
	bool cursorActive_ = true;
};