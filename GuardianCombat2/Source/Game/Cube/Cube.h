//===================================================================================================
//
//		Cube.h
//
//===================================================================================================


#ifndef _3DPOLYGON_H_
#define _3DPOLYGON_H_

#include "../Object.h"
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

	OBB* GetCollision();
	void SetCollision(OBB* collision);

	OBB* AddCollision();
	

private:
	LPDIRECT3DTEXTURE9			pTexture_;
	LPDIRECT3DVERTEXBUFFER9		pVertexBuffer_ = NULL;
	LPDIRECT3DINDEXBUFFER9		pIndexBuffer_ = NULL;
	D3DMATERIAL9				mat_;
	OBB* collision_ = nullptr;
};


#endif // !_3DPOLYGON_H_
