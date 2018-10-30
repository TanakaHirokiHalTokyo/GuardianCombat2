#pragma once

//===========================================================
//
//		PauseÉVÅ[Éì
//				
//===========================================================
#include "Scene.h"

class Pause:public Scene
{
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
};