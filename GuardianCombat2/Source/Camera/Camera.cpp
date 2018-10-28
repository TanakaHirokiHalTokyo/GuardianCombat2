
#include "Camera.h"
#include "../Game/Object.h"
#include "../DirectXRenderer.h"
#include "../Vector3/Vector3.h"

CAMERA_INFO Camera::camInfo_;

Camera::Camera()
	:at_(D3DXVECTOR3(0,0,0))
{
	vector_ = new Vector3();

	D3DXMatrixIdentity(&this->mtxProj_);
	D3DXMatrixIdentity(&this->mtxView_);

	Object::transform_.pos = D3DXVECTOR3(EyeX,EyeY,EyeZ);

	D3DXVec3Normalize(&this->at_,&this->at_);
	vector_->SetFront( this->at_ - this->transform_.pos);

	D3DXVECTOR3 right;
	D3DXVec3Cross(&right,&vector_->GetUp(),&vector_->GetFront());
	D3DXVec3Normalize(&right,&right);
	vector_->SetRight(right);

	this->SetObjectType(Object::obj_Camera);
	this->SetProjection();
}
Camera::~Camera()
{
	SAFE_DELETE(vector_);
}

void Camera::Rotation(float angle)
{
	//前ベクトル
	vector_->SetFront( at_ - transform_.pos);
	//前ベクトル正規化
	D3DXVec3Normalize(&vector_->GetFront(), &vector_->GetFront());

	//右ベクトル
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);

	D3DXMATRIX mtxRotX;
	D3DXMATRIX mtxRotY;
	D3DXVECTOR3 RotVec;

	//回転行列を作成
	D3DXMatrixRotationAxis(&mtxRotX, &vector_->GetRight(), D3DXToRadian(angle));

	//向きベクトル
	RotVec = at_ - transform_.pos;
	//m_CamAt = rightVec * D3DXVec3Length(&RotVec);

	//前ベクトルおよび右ベクトル（上方向ベクトル（上方向が傾いている場合））
	//D3DXVec3TransformNormal　・・・　ベクトルの座標変換
	//(平行移動無視　回転のみ		回転・平行移動させたい場合D3DXVec3TransformCoordをつかう)
	D3DXVec3TransformNormal(
		&RotVec,		//回転させたいベクトル(向きべくとる)
		&RotVec,
		&mtxRotX
	);


	D3DXVECTOR3 OldCamAt;
	OldCamAt = at_;

	at_ = transform_.pos + RotVec;

	vector_->SetFront(at_ - transform_.pos);
	D3DXVec3Normalize(&vector_->GetFront(), &vector_->GetFront());

	D3DXVECTOR3 dir = vector_->GetFront();
	dir.y = 0;			//地を這う準備

	D3DXVec3Normalize(&dir, &dir);

	//単位ベクトル同士の内積まったく同じだった場合 1.0fをかえす
	//内積が45°ずれている場合は0.5f	内積が90°ずれている場合は0.0f		135°の場合　-0.5f
	//前ベクトルとdirの内積 Dot(内積)
	float DotAngle = D3DXVec3Dot(&vector_->GetFront(), &dir);

	if (DotAngle <= 0.5f || DotAngle > 1.0f)
	{
		at_ = OldCamAt;
	}


	dir = vector_->GetFront();
	dir.y = 0;			//地を這う準備

	D3DXVec3Normalize(&dir, &dir);


	//前ベクトル
	vector_->SetFront(at_ - transform_.pos);
	//前ベクトル正規化
	D3DXVec3Normalize(&vector_->GetFront(), &vector_->GetFront());

	//右ベクトル
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);


	//回転行列を作成
	D3DXMatrixRotationY(&mtxRotY, D3DXToRadian(angle));

	//向きベクトル
	RotVec = at_ - transform_.pos;
	//m_CamAt = frontVec * D3DXVec3Length(&RotVec);

	//前ベクトルおよび右ベクトル（上方向ベクトル（上方向が傾いている場合））
	//D3DXVec3TransformNormal　・・・　ベクトルの座標変換
	//(平行移動無視　回転のみ		回転・平行移動させたい場合D3DXVec3TransformCoordをつかう)
	D3DXVec3TransformNormal(
		&RotVec,		//回転させたいベクトル(向きべくとる)
		&RotVec,
		&mtxRotY
	);
	at_ = transform_.pos + RotVec;

	this->SetView();
}
void Camera::FinishCameraMovement()
{
	vector_->SetFront(at_ - transform_.pos);
	D3DXVec3Normalize(&vector_->GetFront(), &vector_->GetFront());

	//右ベクトル
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);
	this->SetView();
	this->SetMainCameraInfo(this->transform_.pos, this->at_, vector_->GetUp(), this->mtxView_, this->mtxProj_);
}
void Camera::SetView()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	//ビュー変換行列 LH・・・左手座標系
	D3DXVECTOR3 eye(transform_.pos.x, transform_.pos.y, transform_.pos.z);		//カメラ座標
	D3DXVECTOR3 at(at_.x, at_.y, at_.z);										//中視点座標　見ている場所
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mtxView_, &eye, &at_, &vector_->GetUp());

	pDevice->SetTransform(D3DTS_VIEW, &mtxView_);
}
void Camera::SetProjection()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	//プロジェクション行列
	D3DXMatrixPerspectiveFovLH(&mtxProj_, D3DXToRadian(Fov), (float)ScreenWidth / ScreenHeight, Proj_Near, Proj_Far);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj_);
}

D3DXVECTOR3 Camera::GetFront() const
{
	return vector_->GetFront();
}

void Camera::SetFront(float x, float y, float z)
{
	vector_->SetFront(x,y,z);
}

void Camera::SetFront(D3DXVECTOR3 front)
{
	vector_->SetFront(front);
}

D3DXVECTOR3 Camera::GetRight() const
{
	return vector_->GetRight();
}

void Camera::SetRight(float x, float y, float z)
{
	vector_->SetRight(x,y,z);
}

void Camera::SetRight(D3DXVECTOR3 right)
{
	vector_->SetRight(right);
}

D3DXVECTOR3 Camera::GetUp() const
{
	return vector_->GetUp();
}

void Camera::SetUp(float x, float y, float z)
{
	vector_->SetUp(x,y,z);
}

void Camera::SetUp(D3DXVECTOR3 up)
{
	vector_->SetUp(up);
}
