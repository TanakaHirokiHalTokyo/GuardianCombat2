#pragma once

#include "Scene.h"

class Player;
class PauseScene;
class Cube;

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
	Player* player_ = nullptr;				//�v���C���[���
	PauseScene* pauseScene_ = nullptr;		//�|�[�Y�V�[�����
	Cube* cube_ = nullptr;
};