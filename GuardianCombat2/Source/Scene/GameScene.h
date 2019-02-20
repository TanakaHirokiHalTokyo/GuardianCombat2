#pragma once

#include "Scene.h"

class Player;
class PauseScene;
class Sound;
class GameCountdown;

class GameScene:public Scene
{
public:
	GameScene();
	~GameScene()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

private:
	bool updateOnce_ = false;							//最初の1フレームだけ更新
	GameCountdown* countDown_ = nullptr;	//カウントダウン制御
	Player* player_ = nullptr;							//プレイヤー情報
	PauseScene* pauseScene_ = nullptr;		//ポーズシーン情報
	Sound* bgm_ = nullptr;
};