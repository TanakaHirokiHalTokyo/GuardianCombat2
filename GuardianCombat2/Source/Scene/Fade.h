#pragma once

#include "../Game/Object.h"

class Texture;

class Fade :public Object
{
public:
	Fade();
	~Fade();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	void FadeIn();
	void FadeOut();
	bool IsFading();

private:
	bool fadeIn_ = false;
	Texture* tex_fade_ = nullptr;
	Texture* tex_loading_ = nullptr;
	float fadeAlpha_ = 255.0f;
	bool isFading_ = false;
};