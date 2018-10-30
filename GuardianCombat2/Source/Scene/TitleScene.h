#pragma once

//====================================================
//
//		TitleScene
//
//====================================================

#include "Scene.h"

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
};