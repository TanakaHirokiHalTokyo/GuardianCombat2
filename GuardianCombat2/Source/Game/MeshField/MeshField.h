#pragma once

#include	<d3d9.h>
#include	<d3dx9.h>
#include	"../Object.h"

const float FIELD_SIZE = 11.0f;

class XModel;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 vBuf;		//バーテックスバッファ
	LPDIRECT3DINDEXBUFFER9	iBuf;		//インデックスバッファ
	int	vNum;							//バーテックス数
	int iNum;							//インデックス数
}MeshF;

class MeshField : public Object
{
public:
	struct VERTEX3D
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	enum Texture_List										//TextureList 
	{
		Tex_Turret,
		Tex_Difficulity,
		Tex_Max,
	};
	enum MeshField_Type							//フィールド種類
	{
		Field_Ground,										//地面
		Field_Wall,											//壁
		Field_Max
	};
	enum Wall_Type										
	{
		Wall_Left,					//左壁
		Wall_Front,					//手前壁
		Wall_Right,					//右壁
		Wall_Back,					//奥壁
		Wall_Up						//上壁
	};

public:
	MeshField();		
	~MeshField();

	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void Draw(LPD3DXEFFECT effect, UINT pass);
	void EndDraw()override;

	float GetSizeX();				//幅サイズ取得
	float GetSizeY();				//奥行サイズ取得
	float GetSpliteX();			//幅分割数取得
	float GetSpliteY();			//奥行分割数取得

	static float GetHeight(D3DXVECTOR3 position);			//指定した座標のフィールドの高さ取得

private:
	void SetMeshField(float SizeX, float SizeY, int SpliteX, int SpliteY, MeshField_Type type);
private:
	LPDIRECT3DTEXTURE9 pTexture_ = nullptr;			//テクスチャポインタ

	static float sizeX_;								//幅サイズ
	static float sizeY_;								//奥行サイズ
	static int spliteX_;								//幅分割数
	static int spliteY_;								//奥行分割数
	static VERTEX3D*	Vertex;					//頂点情報
	static WORD*		pIndex;						//インデックス情報

	D3DMATERIAL9 mat_;					//マテリアル情報
	MeshF meshf_[MeshField::MeshField_Type::Field_Max];												//ワールド行列
	MeshField_Type type_;					//メッシュフィールドの種類
	D3DXMATRIX world_;						//ワールド行列

	XModel* wall_[4] = {};											//壁のモデル
};