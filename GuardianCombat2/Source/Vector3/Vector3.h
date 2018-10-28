#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Vector3
{
public:
	Vector3();
	void SetFront(float x,float y,float z);
	void SetRight(float x,float y,float z);
	void SetUp(float x,float y,float z);
	void SetFront(D3DXVECTOR3 front);
	void SetRight(D3DXVECTOR3 right);
	void SetUp(D3DXVECTOR3 up);
	D3DXVECTOR3 GetFront();
	D3DXVECTOR3 GetRight();
	D3DXVECTOR3 GetUp();
private:
	D3DXVECTOR3 frontVec_;
	D3DXVECTOR3 rightVec_;
	D3DXVECTOR3 upVec_;
};