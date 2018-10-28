#pragma once

#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <d3d9.h>
#include <d3dx9.h>

struct EffectFile
{
	const wchar_t fileName[256];
};


class CEffekseer
{
public:
	struct Transform
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scale;
		D3DXVECTOR3 rotate;
	};

	const enum Effect
	{
		Effect_Core,
		Effect_Portal,
		Effect_Max
	};
	const EffectFile fileName_[Effect_Max]{
		L"resource/Effekseer/Core.efk",
		L"resource/Effekseer/Portal.efk",
	};


public:
	CEffekseer(Effect EffectType);
	~CEffekseer();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	void SetSpeed(float speed);
	void RepeatEffect(bool repeat);
	void Play();
	void Paused(bool paused);
	void Stop();
	bool GetPlaying();
	void SetVisible(bool flag);
	bool GetVisible();
	void SetPosition(float x,float y,float z);
	void SetPosition(D3DXVECTOR3 trans);
	D3DXVECTOR3 GetPosition();
	void SetRotate(float x,float y,float z);
	void SetRotate(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotate();
	void SetScale(float x, float y,float z);
	void SetScale(D3DXVECTOR3 scale);
	D3DXVECTOR3 GetScale();

private:
	void LoadEffect();
	void SetView(D3DXVECTOR3 Pos, D3DXVECTOR3 At, D3DXVECTOR3 Up);
	void SetProj(float Fov, float ScreenWidth, float ScreenHeight, float Near, float Far);
	void SetMatrix(D3DXMATRIX mat);

	Effekseer::Manager*					effekseerManager_;
	EffekseerRendererDX9::Renderer*		effekseerRenderer_;
	Effekseer::Effect*					effekseerEffect_;
	Effekseer::Handle					effekseerHandle_;
	CEffekseer::Effect					effectType_;
	bool repeat_ = false;				//繰りかえし再生するか。
	bool paused_ = false;				//ポーズ中か。
	bool playing_ = false;				//再生中か
	bool visible_ = false;				//表示するか
	float speed_ = 1.0f;
	Transform transform_;

};