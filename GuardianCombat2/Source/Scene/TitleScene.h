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
private:
	XModel* hige_ = nullptr;
	bool up_ = false;
	float value_ = 0.0f;
	CEffekseer* effect_ = nullptr;
	Sound* bgm_ = nullptr;
	TitleSelect* titleSelect_ = nullptr;
};