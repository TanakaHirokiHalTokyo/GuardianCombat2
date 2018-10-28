#pragma once

#include	<d3d9.h>
#include	<d3dx9.h>
#include	"../Object.h"


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
	enum MeshField_Type
	{
		Field_Ground,
		Field_Wall,
		Field_Max
	};
	enum Wall_Type
	{
		Wall_Left,
		Wall_Front,
		Wall_Right,
		Wall_Back,
		Wall_Ups
	};

public:
	MeshField();
	~MeshField();

	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void Draw(LPD3DXEFFECT effect,UINT pass);
	void EndDraw()override;

	float GetSizeX();
	float GetSizeY();
	float GetSpliteX();
	float GetSpliteY();

	static float GetHeight(D3DXVECTOR3 position);

private:
	void SetMeshField(float SizeX, float SizeY, int SpliteX, int SpliteY, MeshField_Type type);
private:
	LPDIRECT3DTEXTURE9 pTexture_;

	static float sizeX_;
	static float sizeY_;
	static int spliteX_;
	static int spliteY_;
	static VERTEX3D*	Vertex;
	static WORD*		pIndex;

	D3DMATERIAL9 mat_;
	MeshF meshf_[MeshField::MeshField_Type::Field_Max];												//ワールド行列
	MeshField_Type type_;
	D3DXMATRIX world_;
};