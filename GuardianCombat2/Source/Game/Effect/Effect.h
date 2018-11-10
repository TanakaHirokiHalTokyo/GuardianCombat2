#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <d3dx9.h>
#include <d3d9.h>

class EffectManager
{
public:
	struct VERTEX3D
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	static void Init();
	static void Uninit();

	static LPDIRECT3DTEXTURE9 GetTexture();						//テクスチャポインタ取得
	static LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer();			//バーテックスバッファ取得
	static LPDIRECT3DINDEXBUFFER9& GetIndexBuffer();				//インデックスバッファ取得
	static VERTEX3D& GetVertex();
private:
	static LPDIRECT3DTEXTURE9 texture_;							//テクスチャポインタ
	static LPDIRECT3DVERTEXBUFFER9 vertexBuffer_;				//バーテックスバッファ
	static LPDIRECT3DINDEXBUFFER9 indexBuffer_;					//インデックスバッファ
	static VERTEX3D* vertex_;
};

class AdditionEffect
{
public:
	static const int EFFECTNUM = 1000;

	struct Transform
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scale;
	};

	AdditionEffect();
	~AdditionEffect();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void CreateEffect(D3DXVECTOR3 pos);
	void SetLimit(int limit);
	void SetColor(float r, float g, float b);
	void SetScale(float size);

private:
	int count_[EFFECTNUM];			//カウンタ
	int limit_ = 60;				//寿命
	bool use_[EFFECTNUM];
	D3DXCOLOR color_[EFFECTNUM] = {};
	Transform transform_[EFFECTNUM] = {};		//トランスフォームデータ
};



#endif // !_EFFECT_H_
