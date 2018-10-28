#pragma once

#include "../Game/Object.h"
class Light:public Object
{
public:
	Light(){
		this->SetObjectType(Object::obj_Light);
	}
	~Light(){}
	virtual void Init(){}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

protected:
	D3DLIGHT9	light_;	
};

class DirectionalLight :public Light
{
public:
	DirectionalLight();
	~DirectionalLight();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	void SetVector(D3DXVECTOR3 vec);
	D3DXVECTOR3 GetVector();
	D3DXVECTOR4 GetAmbient();

private:
	float angle_ = 0.0f;
	D3DXVECTOR3 vector_ = {};
};