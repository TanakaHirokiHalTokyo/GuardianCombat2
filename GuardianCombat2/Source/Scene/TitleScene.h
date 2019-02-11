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
	XModel* hige_ = nullptr;							//�G�̃��f�����
	bool up_ = false;										//�㏸�t���O
	float value_ = 0.0f;									//�㏸�l
	CEffekseer* effect_ = nullptr;					//�G�t�F�N�g���
	Sound* bgm_ = nullptr;							//BGM���
	Sound* selectOKSE_ = nullptr;				//�I�������ʉ�
	TitleSelect* titleSelect_ = nullptr;			//�I���Ǘ��N���X
};