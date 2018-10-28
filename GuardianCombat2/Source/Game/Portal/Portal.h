#pragma once
#include "../Object.h"

class CEffekseer;

class Portal :public Object
{
public:
	Portal();
	~Portal()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

private:
	CEffekseer* portal_ = nullptr;
};