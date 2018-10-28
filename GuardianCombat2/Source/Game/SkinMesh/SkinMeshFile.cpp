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

	// ボーンの行列領域を確保
	AllocateAllBoneMatrix(m_RootFrame);
	m_AnimController->SetTrackSpeed(0, 0.5f);

	for (int i = 0; i < MAX_ANIMATION; i++)
	{
		m_AnimSet[i] = NULL;
	}

	// 追加
	//アニメーショントラックの取得
	for (UINT i = 0; i < m_AnimController->GetNumAnimationSets(); i++)
	{
		//アニメーション取得
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
	// ボーンのマトリクス分の領域を確保してデータを保存する
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

	// 子供があるかどうか
	if (frame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}

	// 兄弟があるかどうか
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
	// フレームの行列を更新
	UpdateFrame(m_RootFrame, matrix);
	// フレーム描画
	DrawFrame(m_RootFrame);
}

void SkinMeshFile::Draw(LPD3DXEFFECT effect, UINT pass, LPD3DXMATRIX matrix)
{
	// フレームの行列を更新
	UpdateFrame(m_RootFrame, matrix);
	// フレーム描画
	DrawFrame(effect, pass, m_RootFrame);
}

void SkinMeshFile::DrawFrame(LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

	// コンテナの数だけ描画する
	while (container_data != NULL)
	{
		DrawMeshContainer(frame, container_data);

		container_data = container_data->pNextMeshContainer;
	}
	
	// 兄弟がいれば再帰で呼び出す
	if (frame_data->pFrameSibling != NULL)
	{
		DrawFrame(frame_data->pFrameSibling);
	}

	// 子がいれば再帰で呼び出す
	if (frame_data->pFrameFirstChild != NULL)
	{
		DrawFrame(frame_data->pFrameFirstChild);
	}
}

void SkinMeshFile::DrawFrame(LPD3DXEFFECT effect, UINT pass, LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

	// コンテナの数だけ描画する
	while (container_data != NULL)
	{
		DrawMeshContainer(effect,pass,frame, container_data);

		container_data = container_data->pNextMeshContainer;
	}

	// 兄弟がいれば再帰で呼び出す
	if (frame_data->pFrameSibling != NULL)
	{

		DrawFrame(effect, pass, frame_data->pFrameSibling);
	}

	// 子がいれば再帰で呼び出す
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

		// ボーンの数まわす
		for(DWORD i = 0; i < original_container->m_BoneNum; i++)
		{
			// ブレンドするボーンの数
			DWORD bone_blend_num = 0;

			// ボーンIDからブレンドする個数を割り出す
			for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
			{
				if (bone_buffer[i].BoneId[j] != UINT_MAX)
				{
					bone_blend_num++;
				}
			}
			
			// 頂点ブレンドの設定
			// 第二引数には_D3DVERTEXBLENDFLAGSが使われているので
			// 対象となっているボーンの数と差異に注意
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

			for(DWORD j = 0; j < original_container->m_BoneWeightNum; j++) 
			{
				DWORD matrix_index = bone_buffer[i].BoneId[j];
				D3DXMATRIX matrix;

				if (matrix_index != UINT_MAX)
				{
					
					// オフセット行列(m_BoneOffsetMatrix) * ボーンの行列(m_BoneMatrix)で最新の位置を割り出す
					matrix = original_container->m_BoneOffsetMatrix[matrix_index] * (*original_container->m_BoneMatrix[matrix_index]);
					pDevice->SetTransform( D3DTS_WORLDMATRIX(j), &matrix );
				}
			}

			pDevice->SetMaterial( &original_container->pMaterials[bone_buffer[i].AttribId].MatD3D );
			pDevice->SetTexture( 0, original_container->m_TextureList[bone_buffer[i].AttribId] );
			original_container->MeshData.pMesh->DrawSubset(i);	
		}
	// 通常のXFile描画
	} 
	else {
		// 描画位置行列の設定
		pDevice->SetTransform(D3DTS_WORLD, &frame_data->m_CombinedTransformationMatrix);

		// メッシュの描画
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

		
		// ボーンの数まわす
		for (DWORD i = 0; i < original_container->m_BoneNum; i++)
		{
			// ブレンドするボーンの数
			DWORD bone_blend_num = 0;

			// ボーンIDからブレンドする個数を割り出す
			for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
			{
				if (bone_buffer[i].BoneId[j] != UINT_MAX)
				{
					bone_blend_num++;
				}
			}

			// 頂点ブレンドの設定
			// 第二引数には_D3DVERTEXBLENDFLAGSが使われているので
			// 対象となっているボーンの数と差異に注意
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

			D3DXMATRIX matrix;
			D3DXMatrixIdentity(&matrix);

			
			for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
			{

				
				DWORD matrix_index = bone_buffer[i].BoneId[j];
				
				if (matrix_index != UINT_MAX)
				{
					//Camera 情報
					CAMERA_INFO& camInfo = Camera::GetCameraInfo();
					D3DXMATRIX wv = matrix * camInfo.view;
					D3DXMATRIX wvp = matrix * camInfo.view * camInfo.proj;	//ワールドビュープロジェクション

					// オフセット行列(m_BoneOffsetMatrix) * ボーンの行列(m_BoneMatrix)で最新の位置を割り出す
					matrix = original_container->m_BoneOffsetMatrix[matrix_index] * (*original_container->m_BoneMatrix[matrix_index]);

					ShadowMapShader::SetInfo(matrix);
					effect->SetMatrix("World", &matrix);
					effect->SetMatrix("WView", &wv);	//Viewセット
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
		
		// 通常のXFile描画
	}
	else {
		// 描画位置行列の設定
		//pDevice->SetTransform(D3DTS_WORLD, &frame_data->m_CombinedTransformationMatrix);

		{
			effect->SetMatrix("World", &frame_data->m_CombinedTransformationMatrix);
		}
		//Camera 情報
		CAMERA_INFO& camInfo = Camera::GetCameraInfo();
		D3DXMATRIX wv = frame_data->m_CombinedTransformationMatrix * camInfo.view;
		D3DXMATRIX wvp = frame_data->m_CombinedTransformationMatrix * camInfo.view * camInfo.proj;	//ワールドビュープロジェクション

		effect->SetMatrix("WView", &wv);	//Viewセット
		effect->SetMatrix("WVProj", &wvp);
		

		// メッシュの描画
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

	// 姿勢行列の更新
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

	// 兄弟があれば再帰で呼び出す
    if (frame->pFrameSibling != NULL)
    {
        UpdateFrame(frame->pFrameSibling, parent_matrix);
    }

	// 子供がいれば再帰で呼び出す
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
		// 合成中か否かを判定
		m_CurrentWeightTime += time;
		if (m_CurrentWeightTime <= m_ShiftTime)
		{
			// 合成中。ウェイトを算出
			float Weight = m_CurrentWeightTime / m_ShiftTime;
			// ウェイトを登録
			m_AnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
			m_AnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
		}
		else
		{
			// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
			m_AnimController->SetTrackWeight(0, 1.0f);       // 現在のアニメーション
			m_AnimController->SetTrackEnable(1, false);      // 前のアニメーションを無効にする
		}

		if (m_bPlayMontage)
		{
			if (m_CurrentWeightTime >= m_MontageTime)
			{
				ChangeAnim(m_NextAnim, 0.3f);
				m_bPlayMontage = false;
			}
		}

		// 0.016秒ずつアニメーションを進める
		m_AnimController->AdvanceTime(time, NULL);
	}
}

// 追加
void SkinMeshFile::ChangeAnim(UINT animID, float shiftTime)
{
	// 存在するアニメーション番号か
	if (animID < m_AnimController->GetNumAnimationSets())
	{
		// 今のアニメーションと違うか
		if (animID != m_CurrentAnim)
		{
			// 現在のアニメーションセットの設定値を取得
			D3DXTRACK_DESC TD;   // トラックの能力
			m_AnimController->GetTrackDesc(0, &TD);

			// 今のアニメーションをトラック1に移行し
			// トラックの設定値も移行
			m_AnimController->SetTrackAnimationSet(1, m_AnimSet[m_CurrentAnim]);
			m_AnimController->SetTrackDesc(1, &TD);

			m_CurrentAnim = animID;
			m_AnimController->SetTrackAnimationSet(0, m_AnimSet[m_CurrentAnim]);

			// トラックの合成を許可
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
	// 指定のアニメーションIDの存在をチェック
	if (animID < m_AnimController->GetNumAnimationSets())
		return false;

	// トラックスピード調節値を算出
	FLOAT DefTime = m_AnimSet[animID]->GetPeriod();
	m_AnimController->SetTrackSpeed(0, DefTime * time);

	return true;
}

void SkinMeshFile::PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID)
{
	// 存在するアニメーション番号か
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
