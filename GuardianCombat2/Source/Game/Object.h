#pragma once


#include	<unordered_map>
#include	<iostream>
#include	<memory>
#include	<d3d9.h>
#include	<d3dx9.h>
#include	"../Scene/Scene.h"
#include	"../Game/GameManager/GameManager.h"

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

	static void InitAll();
	static void UpdateAll();
	static void BeginDrawAll();
	static void DrawAll();
	static void EndDrawAll();
	static void ReleaseAll();
	static void GameObjectReleaseAll();


	void Release();

	//----------------------------------------------------------------------------------------
	//		Positionセット・ゲット関数
	//----------------------------------------------------------------------------------------
	void SetPosition(float x, float y, float z) { this->transform_.pos = D3DXVECTOR3(x, y, z); }
	void SetPosition(D3DXVECTOR3 pos) { this->transform_.pos = pos; }
	void SetPositionX(float x) { this->transform_.pos.x = x; }
	void SetPositionY(float y) { this->transform_.pos.y = y; }
	void SetPositionZ(float z) { this->transform_.pos.z = z; }
	D3DXVECTOR3 GetPosition() { return this->transform_.pos; }

	//---------------------------------------------------------------------------------------
	//		Rotationセット・ゲット関数
	//----------------------------------------------------------------------------------------
	void SetRotation(float x, float y, float z) { this->transform_.rotate = D3DXVECTOR3(x, y, z); }
	void SetRotation(D3DXVECTOR3 rot) { this->transform_.rotate = rot; }
	void SetRotationX(float x) { this->transform_.rotate.x = x; }
	void SetRotationY(float y) { this->transform_.rotate.y = y; }
	void SetRotationZ(float z) { this->transform_.rotate.z = z; }
	D3DXVECTOR3 GetRotate() { return this->transform_.rotate; }

	//----------------------------------------------------------------------------------------
	//		Scaleセット・ゲット関数
	//----------------------------------------------------------------------------------------
	void SetScale(float x, float y, float z) { this->transform_.scale = D3DXVECTOR3(x, y, z); }
	void SetScale(D3DXVECTOR3 scale) { this->transform_.scale = scale; }
	D3DXVECTOR3 GetScale() { return this->transform_.scale; }

	//----------------------------------------------------------------------------------------
	//		オブジェクトタイプセット・ゲット関数
	//----------------------------------------------------------------------------------------
	void SetObjectType(ObjectType objType) { this->type_ = objType; }
	ObjectType GetObjectType() { return this->type_; }

	//----------------------------------------------------------------------------------------
	//		Existセット・ゲット関数		生成した際trueになり　オブジェクト消去時false
	//----------------------------------------------------------------------------------------
	void SetExist(bool exist) { this->exist_ = exist; }
	bool GetExist() { return this->exist_; }

	//----------------------------------------------------------------------------------------
	//		Visibleセット・ゲット関数
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
	//		Shader使うか　フラグ
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
		Scene* scene = GameManager::GetScene();

		if(GameManager::GetSceneTag() == "GameScene")
		{	//ゲームシーンだった場合
			_Object* obj = new _Object(args ...);
			obj->Init();
			gameObjects_.emplace_back(obj);
			return obj;
		}
		else
		{	//ゲームシーンではない場合
			_Object* obj = new _Object(args ...);
			obj->Init();
			objects_.emplace_back(obj);
			return obj;
		}
	}

protected:
	static vector<Object*> gameObjects_;							//ゲームシーンオブジェクト動的配列（ゲーム終了時に解放処理　Load時間削減）
	static vector<Object*> objects_;								//動的配列
	static vector<OBB*> enemyHormingCollisions_;					//ホーミング弾のコリジョン
	static vector<Sphere*> playerCollision_;						//プレイヤーのコリジョン

	bool exist_ = true;												//存在しているか falseになったら消される。
	bool visible_ = true;											//可視状態にするか
	bool pause_ = false;											//ポーズ状態か
	ObjectType type_ = obj_None;									//Objectが何のオブジェクトか
	Transform transform_ = {};
	D3DXMATRIX world_;
	bool useShader_ = false;										//シェーダーをしようするか。
};