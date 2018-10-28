//======================================================================
//	3D�|���S���w�b�_[scene3D]
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

	// �A�j���[�V�����֘A
	SkinMeshFile* GetSkinMeshFile() { return m_SkinMeshFile; }
	void ChangeAnim(UINT animID, float shiftTime);
	bool SetLoopTime(FLOAT time);
	// �A�j���[�V�����̍Đ����x���グ��
	void SetAnimPlaySpeed(float speed) { m_AnimPlaySpeed = m_DefAnimSpeed * speed; }
	// 1��Đ��̃A�j���\�V����
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID);
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID, float playSpeed);
	bool GetPlayMontage() { return m_SkinMeshFile->GetPlayMontage(); }
	// �A�j���[�V�����̍Đ����Ԃ��擾
	float GetWeightTime() { return m_SkinMeshFile->GetWeightTime(); }
	//�@�A�j���[�V����1�񕪂̍Đ����Ԃ��擾
	float GetAnimationTime() { return m_SkinMeshFile->GetAnimationTime(); }

private:
	SkinMeshFile*		m_SkinMeshFile;
	float				m_AnimPlaySpeed;
	float				m_DefAnimSpeed;
	D3DXMATRIX world_;
};

#endif
