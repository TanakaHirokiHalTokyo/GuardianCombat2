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
	void SetModel1(XModel* model);
	void SetModel2(XModel* model);
private:
	Vector3* vector_ = nullptr;
	XModel* model1_ = nullptr;
	XModel* model2_ = nullptr;
};