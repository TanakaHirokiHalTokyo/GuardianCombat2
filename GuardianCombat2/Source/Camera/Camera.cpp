
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
	//�O�x�N�g��
	vector_->SetFront( at_ - transform_.pos);
	//�O�x�N�g�����K��
	D3DXVec3Normalize(&vector_->GetFront(), &vector_->GetFront());

	//�E�x�N�g��
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);

	D3DXMATRIX mtxRotX;
	D3DXMATRIX mtxRotY;
	D3DXVECTOR3 RotVec;

	//��]�s����쐬
	D3DXMatrixRotationAxis(&mtxRotX, &vector_->GetRight(), D3DXToRadian(angle));

	//�����x�N�g��
	RotVec = at_ - transform_.pos;
	//m_CamAt = rightVec * D3DXVec3Length(&RotVec);

	//�O�x�N�g������щE�x�N�g���i������x�N�g���i��������X���Ă���ꍇ�j�j
	//D3DXVec3TransformNormal�@�E�E�E�@�x�N�g���̍��W�ϊ�
	//(���s�ړ������@��]�̂�		��]�E���s�ړ����������ꍇD3DXVec3TransformCoord������)
	D3DXVec3TransformNormal(
		&RotVec,		//��]���������x�N�g��(�����ׂ��Ƃ�)
		&RotVec,
		&mtxRotX
	);


	D3DXVECTOR3 OldCamAt;
	OldCamAt = at_;

	at_ = transform_.pos + RotVec;

	vector_->SetFront(at_ - transform_.pos);
	D3DXVec3Normalize(&vector_->GetFront(), &vector_->GetFront());

	D3DXVECTOR3 dir = vector_->GetFront();
	dir.y = 0;			//�n�𔇂�����

	D3DXVec3Normalize(&dir, &dir);

	//�P�ʃx�N�g�����m�̓��ς܂����������������ꍇ 1.0f��������
	//���ς�45������Ă���ꍇ��0.5f	���ς�90������Ă���ꍇ��0.0f		135���̏ꍇ�@-0.5f
	//�O�x�N�g����dir�̓��� Dot(����)
	float DotAngle = D3DXVec3Dot(&vector_->GetFront(), &dir);

	if (DotAngle <= 0.5f || DotAngle > 1.0f)
	{
		at_ = OldCamAt;
	}


	dir = vector_->GetFront();
	dir.y = 0;			//�n�𔇂�����

	D3DXVec3Normalize(&dir, &dir);


	//�O�x�N�g��
	vector_->SetFront(at_ - transform_.pos);
	//�O�x�N�g�����K��
	D3DXVec3Normalize(&vector_->GetFront(), &vector_->GetFront());

	//�E�x�N�g��
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);


	//��]�s����쐬
	D3DXMatrixRotationY(&mtxRotY, D3DXToRadian(angle));

	//�����x�N�g��
	RotVec = at_ - transform_.pos;
	//m_CamAt = frontVec * D3DXVec3Length(&RotVec);

	//�O�x�N�g������щE�x�N�g���i������x�N�g���i��������X���Ă���ꍇ�j�j
	//D3DXVec3TransformNormal�@�E�E�E�@�x�N�g���̍��W�ϊ�
	//(���s�ړ������@��]�̂�		��]�E���s�ړ����������ꍇD3DXVec3TransformCoord������)
	D3DXVec3TransformNormal(
		&RotVec,		//��]���������x�N�g��(�����ׂ��Ƃ�)
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

	//�E�x�N�g��
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

	//�r���[�ϊ��s�� LH�E�E�E������W�n
	D3DXVECTOR3 eye(transform_.pos.x, transform_.pos.y, transform_.pos.z);		//�J�������W
	D3DXVECTOR3 at(at_.x, at_.y, at_.z);										//�����_���W�@���Ă���ꏊ
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mtxView_, &eye, &at_, &vector_->GetUp());

	pDevice->SetTransform(D3DTS_VIEW, &mtxView_);
}
void Camera::SetProjection()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	//�v���W�F�N�V�����s��
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
