#pragma once

#include "../Object.h"

class Vector3;
class XModel;

class Enemy :public Object
{
public:
	virtual ~Enemy();
	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	void SetLife(float value) { life_ = value; }
	float GetLife() { return life_; }
	void SetSpeed(float value) { speed_ = value; }
	float GetSpeed() { return speed_; }

	XModel* GetModel();
protected:
	float life_ = 100.0f;
	float speed_ = 1.0f;
	Vector3* vector_ = nullptr;
	XModel* model_ = nullptr;
};