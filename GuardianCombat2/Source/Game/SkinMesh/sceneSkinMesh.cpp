
#include "../../main.h"
#include "../../DirectXRenderer.h"	
#include "sceneSkinMesh.h"

static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL);

SceneSkinMesh::SceneSkinMesh(const std::string& modelName)
{
	m_SkinMeshFile = NULL;

	m_SkinMeshFile = new SkinMeshFile();
	m_SkinMeshFile->Load(modelName);
}

SceneSkinMesh::~SceneSkinMesh()
{
	if (m_SkinMeshFile != NULL)
	{
		delete m_SkinMeshFile;
	}
}

void SceneSkinMesh::Init()
{
	SetPosition(0, 0, 0);
	SetScale(1.0f, 1.0f, 1.0f);
	SetRotation(0, 0, 0);

	m_DefAnimSpeed = 0.025f;
	m_AnimPlaySpeed = m_DefAnimSpeed;
}

void SceneSkinMesh::Uninit()
{
	
}

void SceneSkinMesh::Update()
{
	if (m_SkinMeshFile != NULL)
	{
		m_SkinMeshFile->UpdateAnim(m_AnimPlaySpeed);
	}
}

void SceneSkinMesh::BeginDraw()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}
	D3DXMATRIX mtxTrans, mtxScale, mtxRotateX, mtxRotateY, mtxRotateZ;

	D3DXMatrixTranslation(&mtxTrans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixScaling(&mtxScale, transform_.scale.x, transform_.scale.y, transform_.scale.z);
	D3DXMatrixRotationX(&mtxRotateX, D3DXToRadian(transform_.rotate.x));
	D3DXMatrixRotationY(&mtxRotateY, D3DXToRadian(transform_.rotate.y));
	D3DXMatrixRotationZ(&mtxRotateZ, D3DXToRadian(transform_.rotate.z));
	D3DXMatrixIdentity(&world_);

	world_ = mtxScale;
	world_ *= mtxRotateX;
	world_ *= mtxRotateY;
	world_ *= mtxRotateZ;
	world_ *= mtxTrans;
}

void SceneSkinMesh::Draw()
{
	if (!useShader_)
	{
		LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
		if (pDevice == NULL)
		{
			return;
		}

		if (m_SkinMeshFile != NULL)
		{
			pDevice->SetTransform(D3DTS_WORLD, &world_);
			// FVF(今から使用する頂点情報)の設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			m_SkinMeshFile->Draw(&world_);
		}
	}
	
}

void SceneSkinMesh::Draw(LPD3DXEFFECT effect, UINT pass)
{
	if (m_SkinMeshFile != NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
		if (pDevice == NULL)
		{
			return;
		}

		pDevice->SetTransform(D3DTS_WORLD, &world_);

		// FVF(今から使用する頂点情報)の設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		m_SkinMeshFile->Draw(effect, pass, &world_);
	}
	
}

void SceneSkinMesh::EndDraw()
{
}

void SceneSkinMesh::SetWorld(D3DXMATRIX world)
{
	world_ = world;
}
void SceneSkinMesh::ChangeAnim(UINT animID, float shiftTime)
{
	m_SkinMeshFile->ChangeAnim(animID, shiftTime);
	m_AnimPlaySpeed = m_DefAnimSpeed;
}

bool SceneSkinMesh::SetLoopTime(FLOAT time)
{
	return m_SkinMeshFile->SetLoopTime(m_SkinMeshFile->GetCurrentAnim(), time);
}

void SceneSkinMesh::PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID)
{
	m_SkinMeshFile->PlayMontage(animID, shiftTime, playTime, nextAnimID);
}

void SceneSkinMesh::PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID, float playSpeed)
{
	m_SkinMeshFile->PlayMontage(animID, shiftTime, playTime, nextAnimID);
	SetAnimPlaySpeed(playSpeed);
}
