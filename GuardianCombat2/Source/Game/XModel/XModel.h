#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "../Object.h"

struct XMODEL_TYPE
{
	LPD3DXMESH		pMesh;				//メッシュを受け取る変数
	DWORD			nMaterialNum;			//マテリアルの総数
	LPD3DXBUFFER	pMaterial;			//マテリアルの情報を受けとる変数
	LPDIRECT3DTEXTURE9*	pMeshTextures;	// テクスチャ配列　メモリ動的確保
};

struct XFILENAME
{
	char fileName[256];
};
const XFILENAME g_aXFileName[] =
{
	{ "resource/Xmodel/dome303.X" },
	{ "resource/Xmodel/anim/BODY.x" },
	{ "resource/Xmodel/anim/HEAD.x" },
	{ "resource/Xmodel/anim/ARM_R.x" },
	{ "resource/Xmodel/anim/ARM_L.x" },
	{ "resource/Xmodel/anim/ASI.x" },
	{ "resource/Xmodel/core.x" },
};



class XModel :public Object
{
public:
	enum XMODEL
	{
		MODEL_DOME303,
		MODEL_BODY,
		MODEL_HEAD,
		MODEL_ARM_R,
		MODEL_ARM_L,
		MODEL_ASI,
		MODE_CORE,
		MODEL_MAX
	};

public:
	XModel();
	virtual ~XModel();

	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void Draw(LPD3DXEFFECT effect,UINT pass);
	void SetModelType(XModel::XMODEL modeltype) 
	{ 
		this->modelType_ = modeltype; 
		this->LoadXFile(modelType_);
	}
	void EndDraw()override;
	void SetWorld(D3DXMATRIX world);
	D3DXMATRIX GetWorld() { return world_; }
	LPD3DXMESH GetMesh(XMODEL modelType);

	void SetHieral(bool flag);

protected:
	XMODEL_TYPE xmodel[MODEL_MAX];
	XMODEL modelType_;
	bool hieral_ = false;		//階層構造を使うか
	
private:
	bool LoadXFile(XModel::XMODEL modelType);
};