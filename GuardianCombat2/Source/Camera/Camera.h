#pragma once
#include	"../Game/Object.h"

class Vector3;

constexpr float Proj_Near = 0.1f ;											//近視
constexpr float Proj_Far = 45000.0f;										//遠視
constexpr int Fov = 60;														//視野角

constexpr float EyeX = 0.0f;												//カメラ座標X
constexpr float EyeY = 80.0f;												//カメラ座標Y
constexpr float EyeZ = 40.0f;												//カメラ座標Z
constexpr float CamTransSpeed = 0.5f;										//カメラ平行移動速度
constexpr float CamRotationSpeed = 1.0f;									//カメラ回転速度

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

	D3DXVECTOR3 GetFront()const;
	void SetFront(float x, float y, float z);
	void SetFront(D3DXVECTOR3 front);

	D3DXVECTOR3 GetRight()const;
	void SetRight(float x, float y, float z);
	void SetRight(D3DXVECTOR3 right);

	D3DXVECTOR3 GetUp()const;
	void SetUp(float x, float y, float z);
	void SetUp(D3DXVECTOR3 up);

	D3DXVECTOR3 GetAt()const	{ return at_; }
	void SetAt(float x, float y, float z) { this->at_ = D3DXVECTOR3(x,y,z); }
	void SetAt(D3DXVECTOR3 pos) { this->at_ = pos; }


	D3DXMATRIX GetView()const	{ return mtxView_; }
	D3DXMATRIX GetProj()const	{ return mtxProj_; }
	float GetProjNear() const	{ return Proj_Near; }
	float GetProjFar()const		{ return Proj_Far; }
	int GetFov()				{ return Fov; }

	void Rotation(float angle);
	void FinishCameraMovement();
	//-----------------------------------------------------------------------------------------------
	//		CameraInfo メインカメラの情報関数
	//-----------------------------------------------------------------------------------------------
	static CAMERA_INFO GetCameraInfo() { return camInfo_; }

	void SetMainCameraInfo(D3DXVECTOR3 pos, D3DXVECTOR3 at, D3DXVECTOR3 up, D3DMATRIX view, D3DMATRIX proj) {
		camInfo_.pos = pos;
		camInfo_.at = at;
		camInfo_.up = up;
		camInfo_.view = view;
		camInfo_.proj = proj;
	}
	//Pos
	void SetMainCameraPos(float x, float y, float z) { camInfo_.pos = D3DXVECTOR3(x,y,z); }
	void SetMainCameraPos(D3DXVECTOR3 pos) { camInfo_.pos = pos; }
	//At 
	void SetMainCameraAt(float x, float y, float z) { camInfo_.at = D3DXVECTOR3(x,y,z); }
	void SetMainCameraAt(D3DXVECTOR3 at) { camInfo_.at = at; }
	//Up
	void SetMainCameraUp(float x, float y, float z) { camInfo_.up = D3DXVECTOR3(x, y, z); }
	void SetMainCameraUp(D3DXVECTOR3 up) { camInfo_.up = up; }
	//View
	void SetMainCameraView(D3DXMATRIX view) { camInfo_.view = view; }
	//Projection
	void SetMainCameraProj(D3DXMATRIX proj) { camInfo_.proj = proj;}



protected:
	D3DXMATRIX mtxView_;						//ビュー行列
	D3DXMATRIX mtxProj_;						//プロジェクション行列
	Vector3* vector_;
	D3DXVECTOR3	at_;
	static CAMERA_INFO camInfo_;
	void SetView();
	void SetProjection();

};