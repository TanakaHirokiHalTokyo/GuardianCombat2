//======================================================================
//	3Dポリゴンヘッダ[scene3D]
//
//======================================================================
#ifndef _SCENESKINMESH_H_
#define _SCENESKINMESH_H_

#include <string>
#include "SkinMeshFile.h"
#include "../Object.h"

class SceneSkinMesh : public Object
{
public:
	SceneSkinMesh(const std::string& modelName);
	~SceneSkinMesh();
	void Init();
	void Uninit();
	void Update();
	void BeginDraw();
	void Draw();
	void Draw(LPD3DXEFFECT effect, UINT pass);
	void EndDraw();
	void SetWorld(D3DXMATRIX world);

	// アニメーション関連
	SkinMeshFile* GetSkinMeshFile() { return m_SkinMeshFile; }
	void ChangeAnim(UINT animID, float shiftTime);
	bool SetLoopTime(FLOAT time);
	// アニメーションの再生速度を上げる
	void SetAnimPlaySpeed(float speed) { m_AnimPlaySpeed = m_DefAnimSpeed * speed; }
	// 1回再生のアニメ―ション
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID);
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID, float playSpeed);
	bool GetPlayMontage() { return m_SkinMeshFile->GetPlayMontage(); }
	// アニメーションの再生時間を取得
	float GetWeightTime() { return m_SkinMeshFile->GetWeightTime(); }
	//　アニメーション1回分の再生時間を取得
	float GetAnimationTime() { return m_SkinMeshFile->GetAnimationTime(); }

private:
	SkinMeshFile*		m_SkinMeshFile;
	float				m_AnimPlaySpeed;
	float				m_DefAnimSpeed;
	D3DXMATRIX world_;
};

#endif
