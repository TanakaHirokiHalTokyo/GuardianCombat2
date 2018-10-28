#include "Vector3.h"

Vector3::Vector3()
{
	SetUp(0,1.0f,0);
	SetFront(0.0f, 0.0f, 1.0f);
	SetRight(1.0f, 0.0f, 0.0f);

	D3DXVec3Normalize(&this->upVec_, &this->upVec_);
	D3DXVec3Normalize(&this->rightVec_, &this->rightVec_);
	D3DXVec3Normalize(&this->frontVec_, &this->frontVec_);
}

void Vector3::SetFront(float x, float y, float z)
{
	frontVec_ = D3DXVECTOR3(x,y,z);
	D3DXVec3Normalize(&frontVec_,&frontVec_);
}

void Vector3::SetRight(float x, float y, float z)
{
	rightVec_ = D3DXVECTOR3(x, y, z);
	D3DXVec3Normalize(&rightVec_, &rightVec_);
}

void Vector3::SetUp(float x, float y, float z)
{
	upVec_ = D3DXVECTOR3(x, y, z);
	D3DXVec3Normalize(&upVec_, &upVec_);
}

void Vector3::SetFront(D3DXVECTOR3 front)
{
	frontVec_ = front;
	D3DXVec3Normalize(&frontVec_, &frontVec_);
}

void Vector3::SetRight(D3DXVECTOR3 right)
{
	rightVec_ = right;
	D3DXVec3Normalize(&rightVec_, &rightVec_);
}

void Vector3::SetUp(D3DXVECTOR3 up)
{
	upVec_ = up;
	D3DXVec3Normalize(&upVec_, &upVec_);
}

D3DXVECTOR3 Vector3::GetFront()
{
	return frontVec_;
}

D3DXVECTOR3 Vector3::GetRight()
{
	return rightVec_;
}

D3DXVECTOR3 Vector3::GetUp()
{
	return upVec_;
}
