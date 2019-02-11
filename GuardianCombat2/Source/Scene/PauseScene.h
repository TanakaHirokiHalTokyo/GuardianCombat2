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
	bool isEnableClick_ = true;							//�}�E�X�N���b�N�ł̕ҏW�\��
	bool pausing_ = false;									//���݃|�[�Y����
	PauseSelect* pauseSelect_ = nullptr;		//�|�[�Y�I�𐧌�
	Texture* backImage_ = nullptr;					//���e�N�X�`��	
};