#pragma once
#include	"../Game/Object.h"

class Vector3;

constexpr float Proj_Near = 0.01f ;											//近視
constexpr float Proj_Far = 45000.0f;										//遠視
constexpr int Fov = 50;														//視野角

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

	D3DXVECTOR3 GetFront()const;																	   	//前方向ベクトル取得
	void SetFront(float x, float y, float z);																  	//前方向ベクトル設定
	void SetFront(D3DXVECTOR3 front);																	//前方向ベクトル設定

	D3DXVECTOR3 GetRight()const;																		//右方向ベクトル取得
	void SetRight(float x, float y, float z);																	//右方向ベクトル設定
	void SetRight(D3DXVECTOR3 right);																	//右方向ベクトル設定

	D3DXVECTOR3 GetUp()const;																			//上方向ベクトル取得
	void SetUp(float x, float y, float z);																		//上方向ベクトル設定
	void SetUp(D3DXVECTOR3 up);																			//上方向ベクトル設定

	D3DXVECTOR3 GetAt()const	{ return at_; }														//中視点取得
	void SetAt(float x, float y, float z) { this->at_ = D3DXVECTOR3(x,y,z); }			//中視点設定
	void SetAt(D3DXVECTOR3 pos) { this->at_ = pos; }											//中視点設定

	D3DXMATRIX GetView()const	{ return mtxView_; }										//ビュー行列を取得
	D3DXMATRIX GetProj()const	{ return mtxProj_; }												//プロじぇくション行列を取得
	float GetProjNear() const	{ return Proj_Near; }												//Near取得
	float GetProjFar()const		{ return Proj_Far; }													//Far取得
	int GetFov()				{ return Fov; }																		//視野角取得

	//回転
	void Rotation(float angle);
	//カメラの更新終了時に呼び出し
	void FinishCameraMovement();
	//-----------------------------------------------------------------------------------------------
	//		CameraInfo メインカメラの情報関数
	//-----------------------------------------------------------------------------------------------
	static CAMERA_INFO GetCameraInfo() { return camInfo_; }

	//メインカメラの情報を取得
	void SetMainCameraInfo(D3DXVECTOR3 pos, D3DXVECTOR3 at, D3DXVECTOR3 up, D3DMATRIX view, D3DMATRIX proj) {
		camInfo_.pos = pos;
		camInfo_.at = at;
		camInfo_.up = up;
		camInfo_.view = view;
		camInfo_.proj = proj;
	}
	//Pos設定
	void SetMainCameraPos(float x, float y, float z) { camInfo_.pos = D3DXVECTOR3(x,y,z); }
	void SetMainCameraPos(D3DXVECTOR3 pos) { camInfo_.pos = pos; }
	//At 設定
	void SetMainCameraAt(float x, float y, float z) { camInfo_.at = D3DXVECTOR3(x,y,z); }
	void SetMainCameraAt(D3DXVECTOR3 at) { camInfo_.at = at; }
	//Up設定
	void SetMainCameraUp(float x, float y, float z) { camInfo_.up = D3DXVECTOR3(x, y, z); }
	void SetMainCameraUp(D3DXVECTOR3 up) { camInfo_.up = up; }
	//View設定
	void SetMainCameraView(D3DXMATRIX view) { camInfo_.view = view; }
	//Projection設定
	void SetMainCameraProj(D3DXMATRIX proj) { camInfo_.proj = proj;}
	void SetProjection();

protected:
	D3DXMATRIX mtxView_;						//ビュー行列
	D3DXMATRIX mtxProj_;							//プロジェクション行列
	Vector3* vector_;										//ベクトル情報
	D3DXVECTOR3	at_;								//中視点
	static CAMERA_INFO camInfo_;				//カメラ情報

protected:
	void SetView();										//ビュー行列設定
	

};