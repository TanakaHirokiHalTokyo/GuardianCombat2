#pragma once

#include "Scene.h"

class Player;

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
	Player* player_ = nullptr;		//ƒvƒŒƒCƒ„[î•ñ
};