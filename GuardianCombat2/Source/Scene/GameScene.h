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
	bool updateOnce_ = false;							//�ŏ���1�t���[�������X�V
	GameCountdown* countDown_ = nullptr;	//�J�E���g�_�E������
	Player* player_ = nullptr;							//�v���C���[���
	PauseScene* pauseScene_ = nullptr;		//�|�[�Y�V�[�����
	Sound* bgm_ = nullptr;
};