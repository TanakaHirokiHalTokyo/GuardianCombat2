#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <d3dx9.h>
#include <d3d9.h>
#include <string>

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

	static LPDIRECT3DTEXTURE9 GetTexture();									//テクスチャポインタ取得
	static LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer();			//バーテックスバッファ取得
	static LPDIRECT3DINDEXBUFFER9& GetIndexBuffer();				//インデックスバッファ取得
	static VERTEX3D& GetVertex();
private:
	static LPDIRECT3DTEXTURE9 texture_;									//テクスチャポインタ
	static LPDIRECT3DVERTEXBUFFER9 vertexBuffer_;				//バーテックスバッファ
	static LPDIRECT3DINDEXBUFFER9 indexBuffer_;					//インデックスバッファ
	static VERTEX3D* vertex_;
};

class AdditionEffect
{
public:
	static const int EFFECTNUM = 3000;

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

	inline void SetVisible(const bool flag) { visible_ = flag; }													//可視状態設定
	inline bool GetVisible()const { return visible_; }																	//可視状態取得
	void CreateEffect(D3DXVECTOR3 pos);										//エフェクト作成
	void SetLimit(int limit);																//生存時間設定
	inline int GetLimit() { return limit_; }											//生存時間取得
	void SetColor(float r, float g, float b);											//色設定
	void SetScale(float size);															//サイズ設定
	inline float GetScale() { return transform_[0].scale.x; }				//サイズ取得
	inline D3DXCOLOR GetColor() { return color_[0]; }					//カラー取得
	void SaveParameter(std::string filename);										//パラメータ情報ファイルかき出し
	void LoadParameter(std::string filename);										//パラメータ情報ファイル読み込み
private:
	bool visible_ = false;						//可視状態
	int count_[EFFECTNUM];				//カウンタ
	int limit_ = 60;									//寿命
	bool use_[EFFECTNUM];				//現在使用中か
	D3DXCOLOR color_[EFFECTNUM] = {};		//色
	Transform transform_[EFFECTNUM] = {};		//トランスフォームデータ
};



#endif // !_EFFECT_H_
