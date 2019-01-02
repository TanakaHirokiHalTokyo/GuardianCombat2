#pragma once
#include	"../Game/Object.h"

class Vector3;

constexpr float Proj_Near = 0.01f ;											//�ߎ�
constexpr float Proj_Far = 45000.0f;										//����
constexpr int Fov = 50;														//����p

constexpr float EyeX = 0.0f;												//�J�������WX
constexpr float EyeY = 80.0f;												//�J�������WY
constexpr float EyeZ = 40.0f;												//�J�������WZ
constexpr float CamTransSpeed = 0.5f;										//�J�������s�ړ����x
constexpr float CamRotationSpeed = 1.0f;									//�J������]���x

struct CAMERA_INFO
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 at;
	D3DXVECTOR3 up;
	D3DXMATRIX view;
	D3DXMATRIX proj;
};

class Camera:public Object
{
public:
	Camera();
	~Camera();

	virtual void Init(){}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	D3DXVECTOR3 GetFront()const;																	   	//�O�����x�N�g���擾
	void SetFront(float x, float y, float z);																  	//�O�����x�N�g���ݒ�
	void SetFront(D3DXVECTOR3 front);																	//�O�����x�N�g���ݒ�

	D3DXVECTOR3 GetRight()const;																		//�E�����x�N�g���擾
	void SetRight(float x, float y, float z);																	//�E�����x�N�g���ݒ�
	void SetRight(D3DXVECTOR3 right);																	//�E�����x�N�g���ݒ�

	D3DXVECTOR3 GetUp()const;																			//������x�N�g���擾
	void SetUp(float x, float y, float z);																		//������x�N�g���ݒ�
	void SetUp(D3DXVECTOR3 up);																			//������x�N�g���ݒ�

	D3DXVECTOR3 GetAt()const	{ return at_; }														//�����_�擾
	void SetAt(float x, float y, float z) { this->at_ = D3DXVECTOR3(x,y,z); }			//�����_�ݒ�
	void SetAt(D3DXVECTOR3 pos) { this->at_ = pos; }											//�����_�ݒ�

	D3DXMATRIX GetView()const	{ return mtxView_; }										//�r���[�s����擾
	D3DXMATRIX GetProj()const	{ return mtxProj_; }												//�v���������V�����s����擾
	float GetProjNear() const	{ return Proj_Near; }												//Near�擾
	float GetProjFar()const		{ return Proj_Far; }													//Far�擾
	int GetFov()				{ return Fov; }																		//����p�擾

	//��]
	void Rotation(float angle);
	//�J�����̍X�V�I�����ɌĂяo��
	void FinishCameraMovement();
	//-----------------------------------------------------------------------------------------------
	//		CameraInfo ���C���J�����̏��֐�
	//-----------------------------------------------------------------------------------------------
	static CAMERA_INFO GetCameraInfo() { return camInfo_; }

	//���C���J�����̏����擾
	void SetMainCameraInfo(D3DXVECTOR3 pos, D3DXVECTOR3 at, D3DXVECTOR3 up, D3DMATRIX view, D3DMATRIX proj) {
		camInfo_.pos = pos;
		camInfo_.at = at;
		camInfo_.up = up;
		camInfo_.view = view;
		camInfo_.proj = proj;
	}
	//Pos�ݒ�
	void SetMainCameraPos(float x, float y, float z) { camInfo_.pos = D3DXVECTOR3(x,y,z); }
	void SetMainCameraPos(D3DXVECTOR3 pos) { camInfo_.pos = pos; }
	//At �ݒ�
	void SetMainCameraAt(float x, float y, float z) { camInfo_.at = D3DXVECTOR3(x,y,z); }
	void SetMainCameraAt(D3DXVECTOR3 at) { camInfo_.at = at; }
	//Up�ݒ�
	void SetMainCameraUp(float x, float y, float z) { camInfo_.up = D3DXVECTOR3(x, y, z); }
	void SetMainCameraUp(D3DXVECTOR3 up) { camInfo_.up = up; }
	//View�ݒ�
	void SetMainCameraView(D3DXMATRIX view) { camInfo_.view = view; }
	//Projection�ݒ�
	void SetMainCameraProj(D3DXMATRIX proj) { camInfo_.proj = proj;}
	void SetProjection();

protected:
	D3DXMATRIX mtxView_;						//�r���[�s��
	D3DXMATRIX mtxProj_;							//�v���W�F�N�V�����s��
	Vector3* vector_;										//�x�N�g�����
	D3DXVECTOR3	at_;								//�����_
	static CAMERA_INFO camInfo_;				//�J�������

protected:
	void SetView();										//�r���[�s��ݒ�
	

};