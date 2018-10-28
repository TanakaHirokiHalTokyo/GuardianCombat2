#include <map>
#include "../../DirectXRenderer.h"
#include "../../main.h"
#include "SkinMeshFile.h"
#include "../Shader/ShadowMapShader.h"
#include "../Shader/ToonShader.h"
#include "../Shader/OutlineShader.h"
#include "../../Camera/Camera.h"

SkinMeshFile::~SkinMeshFile()
{
	if (m_SkinMeshData != NULL)
	{
		m_SkinMeshData->DestroyFrame(m_RootFrame);
		SAFE_DELETE(m_SkinMeshData);
	}
	if (m_AnimController != NULL)
	{
		SAFE_RELEASE(m_AnimController);
	}
	for (int i = 0; i < MAX_ANIMATION; i++)
	{
		if (m_AnimSet[i] != NULL)
		{
			SAFE_RELEASE(m_AnimSet[i]);
		}
	}
}

bool SkinMeshFile::Load(std::string file_name)
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}
	

	m_SkinMeshData = new SkinMeshData();

	if (FAILED(D3DXLoadMeshHierarchyFromXA(
		file_name.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		m_SkinMeshData,
		NULL,
		&m_RootFrame,
		&m_AnimController) ) )
	{
		return false;
	}

	// �{�[���̍s��̈���m��
	AllocateAllBoneMatrix(m_RootFrame);
	m_AnimController->SetTrackSpeed(0, 0.5f);

	for (int i = 0; i < MAX_ANIMATION; i++)
	{
		m_AnimSet[i] = NULL;
	}

	// �ǉ�
	//�A�j���[�V�����g���b�N�̎擾
	for (UINT i = 0; i < m_AnimController->GetNumAnimationSets(); i++)
	{
		//�A�j���[�V�����擾
		m_AnimController->GetAnimationSet(i, &(m_AnimSet[i]));
	}
	m_CurrentAnim = 0;
	m_AnimController->GetTrackDesc(0, &m_CurrentTrackDesc);

	m_bPlayMontage = false;


	return true;
}

HRESULT SkinMeshFile::AllocateBoneMatrix(LPD3DXMESHCONTAINER container)
{
    FrameData *pFrame=NULL;
	DWORD bone_num = 0;

    MeshContainer *original_container = (MeshContainer*)container;
    if (original_container->pSkinInfo == NULL)
	{
		return S_OK;
	}

	bone_num = original_container->pSkinInfo->GetNumBones();
	// �{�[���̃}�g���N�X���̗̈���m�ۂ��ăf�[�^��ۑ�����
    original_container->m_BoneMatrix = new D3DXMATRIX*[bone_num];
    for (DWORD i = 0; i < bone_num; i++)
    {
		pFrame = (FrameData*)D3DXFrameFind( m_RootFrame, container->pSkinInfo->GetBoneName(i) );
        if (pFrame == NULL)
		{
			return E_FAIL;
		}
		original_container->m_BoneMatrix[i] = &pFrame->m_CombinedTransformationMatrix;
	}   

	return S_OK;
}

HRESULT SkinMeshFile::AllocateAllBoneMatrix(LPD3DXFRAME frame)
{
	if (frame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(frame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}

	// �q�������邩�ǂ���
	if (frame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}

	// �Z�킪���邩�ǂ���
	if (frame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameSibling)))
		{
			return E_FAIL;
		}		
	}

	return S_OK;
}


void SkinMeshFile::Draw(LPD3DXMATRIX matrix)
{
	// �t���[���̍s����X�V
	UpdateFrame(m_RootFrame, matrix);
	// �t���[���`��
	DrawFrame(m_RootFrame);
}

void SkinMeshFile::Draw(LPD3DXEFFECT effect, UINT pass, LPD3DXMATRIX matrix)
{
	// �t���[���̍s����X�V
	UpdateFrame(m_RootFrame, matrix);
	// �t���[���`��
	DrawFrame(effect, pass, m_RootFrame);
}

void SkinMeshFile::DrawFrame(LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

	// �R���e�i�̐������`�悷��
	while (container_data != NULL)
	{
		DrawMeshContainer(frame, container_data);

		container_data = container_data->pNextMeshContainer;
	}
	
	// �Z�킪����΍ċA�ŌĂяo��
	if (frame_data->pFrameSibling != NULL)
	{
		DrawFrame(frame_data->pFrameSibling);
	}

	// �q������΍ċA�ŌĂяo��
	if (frame_data->pFrameFirstChild != NULL)
	{
		DrawFrame(frame_data->pFrameFirstChild);
	}
}

void SkinMeshFile::DrawFrame(LPD3DXEFFECT effect, UINT pass, LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

	// �R���e�i�̐������`�悷��
	while (container_data != NULL)
	{
		DrawMeshContainer(effect,pass,frame, container_data);

		container_data = container_data->pNextMeshContainer;
	}

	// �Z�킪����΍ċA�ŌĂяo��
	if (frame_data->pFrameSibling != NULL)
	{

		DrawFrame(effect, pass, frame_data->pFrameSibling);
	}

	// �q������΍ċA�ŌĂяo��
	if (frame_data->pFrameFirstChild != NULL)
	{
		DrawFrame(effect, pass, frame_data->pFrameFirstChild);
	}
}
	
void SkinMeshFile::DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container)
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

    FrameData *frame_data = (FrameData*)frame;
    MeshContainer *original_container = (MeshContainer*)container;

	

	if (original_container->pSkinInfo != NULL)
	{
		LPD3DXBONECOMBINATION bone_buffer = (LPD3DXBONECOMBINATION)original_container->m_BoneBuffer->GetBufferPointer();	

		// �{�[���̐��܂킷
		for(DWORD i = 0; i < original_container->m_BoneNum; i++)
		{
			// �u�����h����{�[���̐�
			DWORD bone_blend_num = 0;

			// �{�[��ID����u�����h�����������o��
			for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
			{
				if (bone_buffer[i].BoneId[j] != UINT_MAX)
				{
					bone_blend_num++;
				}
			}
			
			// ���_�u�����h�̐ݒ�
			// �������ɂ�_D3DVERTEXBLENDFLAGS���g���Ă���̂�
			// �ΏۂƂȂ��Ă���{�[���̐��ƍ��قɒ���
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

			for(DWORD j = 0; j < original_container->m_BoneWeightNum; j++) 
			{
				DWORD matrix_index = bone_buffer[i].BoneId[j];
				D3DXMATRIX matrix;

				if (matrix_index != UINT_MAX)
				{
					
					// �I�t�Z�b�g�s��(m_BoneOffsetMatrix) * �{�[���̍s��(m_BoneMatrix)�ōŐV�̈ʒu������o��
					matrix = original_container->m_BoneOffsetMatrix[matrix_index] * (*original_container->m_BoneMatrix[matrix_index]);
					pDevice->SetTransform( D3DTS_WORLDMATRIX(j), &matrix );
				}
			}

			pDevice->SetMaterial( &original_container->pMaterials[bone_buffer[i].AttribId].MatD3D );
			pDevice->SetTexture( 0, original_container->m_TextureList[bone_buffer[i].AttribId] );
			original_container->MeshData.pMesh->DrawSubset(i);	
		}
	// �ʏ��XFile�`��
	} 
	else {
		// �`��ʒu�s��̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &frame_data->m_CombinedTransformationMatrix);

		// ���b�V���̕`��
		for (UINT i = 0; i < original_container->NumMaterials; i++)
		{
			pDevice->SetMaterial( &original_container->pMaterials[i].MatD3D );
			pDevice->SetTexture( 0, original_container->m_TextureList[i] );
			original_container->MeshData.pMesh->DrawSubset(i);
		}
	}

}

void SkinMeshFile::DrawMeshContainer(LPD3DXEFFECT effect, UINT pass, LPD3DXFRAME frame, LPD3DXMESHCONTAINER container)
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	FrameData *frame_data = (FrameData*)frame;
	MeshContainer *original_container = (MeshContainer*)container;

	effect->Begin(NULL, 0);
	effect->BeginPass(pass);

	if (original_container->pSkinInfo != NULL)
	{
		LPD3DXBONECOMBINATION bone_buffer = (LPD3DXBONECOMBINATION)original_container->m_BoneBuffer->GetBufferPointer();

		
		// �{�[���̐��܂킷
		for (DWORD i = 0; i < original_container->m_BoneNum; i++)
		{
			// �u�����h����{�[���̐�
			DWORD bone_blend_num = 0;

			// �{�[��ID����u�����h�����������o��
			for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
			{
				if (bone_buffer[i].BoneId[j] != UINT_MAX)
				{
					bone_blend_num++;
				}
			}

			// ���_�u�����h�̐ݒ�
			// �������ɂ�_D3DVERTEXBLENDFLAGS���g���Ă���̂�
			// �ΏۂƂȂ��Ă���{�[���̐��ƍ��قɒ���
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

			D3DXMATRIX matrix;
			D3DXMatrixIdentity(&matrix);

			
			for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
			{

				
				DWORD matrix_index = bone_buffer[i].BoneId[j];
				
				if (matrix_index != UINT_MAX)
				{
					//Camera ���
					CAMERA_INFO& camInfo = Camera::GetCameraInfo();
					D3DXMATRIX wv = matrix * camInfo.view;
					D3DXMATRIX wvp = matrix * camInfo.view * camInfo.proj;	//���[���h�r���[�v���W�F�N�V����

					// �I�t�Z�b�g�s��(m_BoneOffsetMatrix) * �{�[���̍s��(m_BoneMatrix)�ōŐV�̈ʒu������o��
					matrix = original_container->m_BoneOffsetMatrix[matrix_index] * (*original_container->m_BoneMatrix[matrix_index]);

					ShadowMapShader::SetInfo(matrix);
					effect->SetMatrix("World", &matrix);
					effect->SetMatrix("WView", &wv);	//View�Z�b�g
					effect->SetMatrix("WVProj", &wvp);
					
				}
				
			}

			effect->SetVector("Diffuse", &D3DXVECTOR4(
				original_container->pMaterials[bone_buffer[i].AttribId].MatD3D.Diffuse.r,
				original_container->pMaterials[bone_buffer[i].AttribId].MatD3D.Diffuse.g,
				original_container->pMaterials[bone_buffer[i].AttribId].MatD3D.Diffuse.b,
				original_container->pMaterials[bone_buffer[i].AttribId].MatD3D.Diffuse.a
			));
			effect->SetTexture("MeshTex", original_container->m_TextureList[bone_buffer[i].AttribId]);
			effect->CommitChanges();

			original_container->MeshData.pMesh->DrawSubset(i);
		}
		
		// �ʏ��XFile�`��
	}
	else {
		// �`��ʒu�s��̐ݒ�
		//pDevice->SetTransform(D3DTS_WORLD, &frame_data->m_CombinedTransformationMatrix);

		{
			effect->SetMatrix("World", &frame_data->m_CombinedTransformationMatrix);
		}
		//Camera ���
		CAMERA_INFO& camInfo = Camera::GetCameraInfo();
		D3DXMATRIX wv = frame_data->m_CombinedTransformationMatrix * camInfo.view;
		D3DXMATRIX wvp = frame_data->m_CombinedTransformationMatrix * camInfo.view * camInfo.proj;	//���[���h�r���[�v���W�F�N�V����

		effect->SetMatrix("WView", &wv);	//View�Z�b�g
		effect->SetMatrix("WVProj", &wvp);
		

		// ���b�V���̕`��
		for (UINT i = 0; i < original_container->NumMaterials; i++)
		{
			effect->SetVector("Diffuse", &D3DXVECTOR4(original_container->pMaterials[i].MatD3D.Diffuse.r,
				original_container->pMaterials[i].MatD3D.Diffuse.g,
				original_container->pMaterials[i].MatD3D.Diffuse.b,
				original_container->pMaterials[i].MatD3D.Diffuse.a));
			effect->SetTexture("MeshTex", original_container->m_TextureList[i]);
			effect->CommitChanges();

			original_container->MeshData.pMesh->DrawSubset(i);
		}
	}
	effect->EndPass();
	effect->End();
}

void SkinMeshFile::UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix)
{
	FrameData *frame = (FrameData*)base;

	// �p���s��̍X�V
    if (parent_matrix != NULL)
	{
        D3DXMatrixMultiply(&frame->m_CombinedTransformationMatrix, 
						&frame->TransformationMatrix,
						parent_matrix);
	}
    else
	{
        frame->m_CombinedTransformationMatrix = frame->TransformationMatrix;
	}

	// �Z�킪����΍ċA�ŌĂяo��
    if (frame->pFrameSibling != NULL)
    {
        UpdateFrame(frame->pFrameSibling, parent_matrix);
    }

	// �q��������΍ċA�ŌĂяo��
    if (frame->pFrameFirstChild != NULL)
    {
        UpdateFrame(frame->pFrameFirstChild,
							&frame->m_CombinedTransformationMatrix);
    }
}

void SkinMeshFile::UpdateAnim(float time)
{
	if (m_AnimController != NULL)
	{
		// ���������ۂ��𔻒�
		m_CurrentWeightTime += time;
		if (m_CurrentWeightTime <= m_ShiftTime)
		{
			// �������B�E�F�C�g���Z�o
			float Weight = m_CurrentWeightTime / m_ShiftTime;
			// �E�F�C�g��o�^
			m_AnimController->SetTrackWeight(0, Weight);       // ���݂̃A�j���[�V����
			m_AnimController->SetTrackWeight(1, 1 - Weight);   // �O�̃A�j���[�V����
		}
		else
		{
			// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
			m_AnimController->SetTrackWeight(0, 1.0f);       // ���݂̃A�j���[�V����
			m_AnimController->SetTrackEnable(1, false);      // �O�̃A�j���[�V�����𖳌��ɂ���
		}

		if (m_bPlayMontage)
		{
			if (m_CurrentWeightTime >= m_MontageTime)
			{
				ChangeAnim(m_NextAnim, 0.3f);
				m_bPlayMontage = false;
			}
		}

		// 0.016�b���A�j���[�V������i�߂�
		m_AnimController->AdvanceTime(time, NULL);
	}
}

// �ǉ�
void SkinMeshFile::ChangeAnim(UINT animID, float shiftTime)
{
	// ���݂���A�j���[�V�����ԍ���
	if (animID < m_AnimController->GetNumAnimationSets())
	{
		// ���̃A�j���[�V�����ƈႤ��
		if (animID != m_CurrentAnim)
		{
			// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
			D3DXTRACK_DESC TD;   // �g���b�N�̔\��
			m_AnimController->GetTrackDesc(0, &TD);

			// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
			// �g���b�N�̐ݒ�l���ڍs
			m_AnimController->SetTrackAnimationSet(1, m_AnimSet[m_CurrentAnim]);
			m_AnimController->SetTrackDesc(1, &TD);

			m_CurrentAnim = animID;
			m_AnimController->SetTrackAnimationSet(0, m_AnimSet[m_CurrentAnim]);

			// �g���b�N�̍���������
			m_AnimController->SetTrackEnable(0, true);
			m_AnimController->SetTrackEnable(1, true);

			m_CurrentWeightTime = 0.0f;

			SetShiftTime(shiftTime);

			D3DXTRACK_DESC CurrentTrackDesc;
			m_AnimController->GetTrackDesc(0, &CurrentTrackDesc);
			CurrentTrackDesc.Position = 0;
			m_AnimController->SetTrackDesc(0, &CurrentTrackDesc);
		}
	}
}

bool SkinMeshFile::SetLoopTime(UINT animID, FLOAT time)
{
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (animID < m_AnimController->GetNumAnimationSets())
		return false;

	// �g���b�N�X�s�[�h���ߒl���Z�o
	FLOAT DefTime = m_AnimSet[animID]->GetPeriod();
	m_AnimController->SetTrackSpeed(0, DefTime * time);

	return true;
}

void SkinMeshFile::PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID)
{
	// ���݂���A�j���[�V�����ԍ���
	if (animID < m_AnimController->GetNumAnimationSets())
	{
		if (nextAnimID < m_AnimController->GetNumAnimationSets())
		{
			ChangeAnim(animID, shiftTime);

			m_bPlayMontage = true;
			m_MontageTime = playTime;
			m_NextAnim = nextAnimID;
		}
	}
}
