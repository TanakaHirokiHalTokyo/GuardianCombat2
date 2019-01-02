//===================================================================================================
//
//		Cube.h
//
//===================================================================================================


#ifndef _3DPOLYGON_H_
#define _3DPOLYGON_H_

#include "../Object.h"
#include <iostream>
#include <fstream>
class OBB;
//=================================================================================
//		define
//=================================================================================
#define VERTEX_NUM		(24)																	//頂点数
#define INDEX_NUM		(36)																	//インデックス数
//=================================================================================

class Vector3;

class Cube :public Object
{
public:

	struct VERTEX3D
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};
public:
	Cube();
	~Cube()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void Draw(LPD3DXEFFECT effect, UINT pass);
	void EndDraw()override;

	OBB* GetCollision();								//コリジョン取得
	void SetCollision(OBB* collision);			//コリジョン設定

	OBB* AddCollision();								//コリジョン追加
	float GetAttackValue();							//攻撃力取得
	bool GetHit();											//すでに当たっているのかを取得
	void SetHit(bool flag);								//当たった判定設定
	void Hit();												//当たった時に呼び出し
	void SaveParameter(const std::string filename);		//メンバ変数ファイルかき出し
	void LoadParameter(const std::string filename);		//変数ファイルに書き出したデータ読み込み
private:
	const float ATTACK_VALUE = 10.0f;				//基本攻撃力

	LPDIRECT3DTEXTURE9			pTexture_ = nullptr;									//テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9		pVertexBuffer_ = NULL;					//頂点バッファ
	LPDIRECT3DINDEXBUFFER9		pIndexBuffer_ = NULL;					//インデックスバッファ
	D3DMATERIAL9				mat_ = {};														//マテリアル情報
	OBB* collision_ = nullptr;																		//コリジョン
	float attack_ = ATTACK_VALUE;															//攻撃力
	bool isHit = false;																					//すでに当たったか
};


#endif // !_3DPOLYGON_H_
