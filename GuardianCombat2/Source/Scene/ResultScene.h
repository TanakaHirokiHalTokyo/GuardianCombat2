#pragma once


//=========================================================
//
//		ResultScene
//
//
//=========================================================

#include "Scene.h"

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
};