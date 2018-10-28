#pragma once

#include "Scene.h"

class XModel;
class Fade;
class Player;
class Enemy;
class PlayerCamera;
class EnemyAnimation;
class MeshField;
class Core;

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
	Fade* fade_ = nullptr;
	XModel* model_ = nullptr;
	Enemy* enemy_ = nullptr;
	MeshField* field_ = nullptr;
	PlayerCamera* playerCamera_ = nullptr;
	Player* player_ = nullptr;
	Core* core_ = nullptr;
};