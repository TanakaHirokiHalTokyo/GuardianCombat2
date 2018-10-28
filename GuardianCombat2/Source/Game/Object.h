#pragma once


#include	<unordered_map>
#include	<iostream>
#include	<memory>
#include	<d3d9.h>
#include	<d3dx9.h>

#include "../Collision/Collision.h"

constexpr float EPLENGTH = 1000.0f;

class Sphere;
class OBB;
class Enemy;
using namespace std;

class Object
{
public:

	enum ObjectType
	{
		obj_None = 0,
		obj_MainCamera,
		obj_Camera,
		obj_PlayerCamera,
		obj_PlayerModel,
		obj_PlayerBullet,
		obj_Enemy,
		obj_Player,
		obj_Light,
		obj_Field,
		obj_Skydome,
		obj_XModel,
	};

	struct Transform
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scale;
		D3DXVECTOR3 rotate;
	};

public:
	Object();
	virtual ~Object()
	{
	}
	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	static void UpdateAll();
	static void BeginDrawAll();
	static void DrawAll();
	static void EndDrawAll();
	static void ReleaseAll();


	void Release();

	//----------------------------------------------------------------------------------------
	//		Position�Z�b�g�E�Q�b�g�֐�
	//----------------------------------------------------------------------------------------
	void SetPosition(float x, float y, float z) { this->transform_.pos = D3DXVECTOR3(x, y, z); }
	void SetPosition(D3DXVECTOR3 pos) { this->transform_.pos = pos; }
	D3DXVECTOR3 GetPosition() { return this->transform_.pos; }

	//---------------------------------------------------------------------------------------
	//		Rotation�Z�b�g�E�Q�b�g�֐�
	//----------------------------------------------------------------------------------------
	void SetRotation(float x, float y, float z) { this->transform_.rotate = D3DXVECTOR3(x, y, z); }
	void SetRotation(D3DXVECTOR3 rot) { this->transform_.rotate = rot; }
	D3DXVECTOR3 GetRotate() { return this->transform_.rotate; }

	//----------------------------------------------------------------------------------------
	//		Scale�Z�b�g�E�Q�b�g�֐�
	//----------------------------------------------------------------------------------------
	void SetScale(float x, float y, float z) { this->transform_.scale = D3DXVECTOR3(x, y, z); }
	void SetScale(D3DXVECTOR3 scale) { this->transform_.scale = scale; }
	D3DXVECTOR3 GetScale() { return this->transform_.scale; }

	//----------------------------------------------------------------------------------------
	//		�I�u�W�F�N�g�^�C�v�Z�b�g�E�Q�b�g�֐�
	//----------------------------------------------------------------------------------------
	void SetObjectType(ObjectType objType) { this->type_ = objType; }
	ObjectType GetObjectType() { return this->type_; }

	//----------------------------------------------------------------------------------------
	//		Exist�Z�b�g�E�Q�b�g�֐�		����������true�ɂȂ�@�I�u�W�F�N�g������false
	//----------------------------------------------------------------------------------------
	void SetExist(bool exist) { this->exist_ = exist; }
	bool GetExist() { return this->exist_; }

	//----------------------------------------------------------------------------------------
	//		Visible�Z�b�g�E�Q�b�g�֐�
	//----------------------------------------------------------------------------------------
	void SetVisible(bool visible) { this->visible_ = visible; }
	bool GetVisible() { return this->visible_; }

	//----------------------------------------------------------------------------------------
	//		Judgement 
	//----------------------------------------------------------------------------------------
	static void JudgementAll();
	static void CollisionReleaseAll();

	//----------------------------------------------------------------------------------------
	//		PauseUpdate
	//----------------------------------------------------------------------------------------
	void SetPause(bool flag) { this->pause_ = flag; }
	bool GetPause() { return this->pause_; }
	static void SetPauseAll(bool flag);

	//----------------------------------------------------------------------------------------
	//		Shader�g�����@�t���O
	//----------------------------------------------------------------------------------------
	void SetUseShader(bool flag);
	bool GetUseShader();

	void SetWorld(D3DXMATRIX* world);
	D3DXMATRIX* GetWorld();

	static void ObjectReserve(int reserve)
	{
		objects_.reserve(reserve);
	}

	template<class _Object, class ... Args>
	static _Object* Create(Args ... args)
	{
		_Object* obj = new _Object(args ...);
		obj->Init();
		objects_.emplace_back(obj);
		return obj;
	}

protected:
	static vector<Object*> objects_;								//���I�z��

	bool exist_ = true;												//���݂��Ă��邩 false�ɂȂ�����������B
	bool visible_ = true;											//����Ԃɂ��邩
	bool pause_ = false;											//�|�[�Y��Ԃ�
	ObjectType type_ = obj_None;									//Object�����̃I�u�W�F�N�g��
	Transform transform_ = {};
	D3DXMATRIX world_;
	bool useShader_ = false;										//�V�F�[�_�[�����悤���邩�B
};