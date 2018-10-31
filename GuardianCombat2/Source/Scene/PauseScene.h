#pragma once

//========================================================================
//
//		PauseScene
//
//========================================================================

#include "Scene.h"

class Texture;

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
private:
	bool pausing_ = false;					//���݃|�[�Y����
	Texture* pauseWord_ = nullptr;			//�|�[�Y�����e�N�X�`��
};