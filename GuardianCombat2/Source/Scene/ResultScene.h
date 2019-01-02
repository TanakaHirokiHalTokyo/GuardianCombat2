#pragma once


//=========================================================
//
//		ResultScene
//
//
//=========================================================

#include "Scene.h"

class Sound;

class ResultScene :public Scene
{
public:
	ResultScene();
	~ResultScene();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	int count_ = 0;
	Sound* bgm_ = nullptr;
};