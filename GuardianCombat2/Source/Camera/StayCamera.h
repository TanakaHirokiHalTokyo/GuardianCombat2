#pragma once
#include "Camera.h"

class Vector3;
class XModel;

class StayCamera :public Camera
{
public:
	StayCamera();
	~StayCamera()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	Vector3* vector_ = nullptr;

};