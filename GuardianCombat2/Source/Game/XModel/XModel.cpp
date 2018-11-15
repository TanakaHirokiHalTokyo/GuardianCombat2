

#include "XModel.h"
#include "../../DirectXRenderer.h"
#include "../../Camera/Camera.h"
#include "../../Imgui/ImguiManager.h"
#include "../../DInput/DirectInput.h"

XModel::XModel()
{
	

}
XModel::~XModel()
{
	if (this->xmodel[modelType_].pMesh != NULL)
	{
		this->xmodel[modelType_].pMesh->Release();
		this->xmodel[modelType_].pMesh = NULL;
	}
	if (this->xmodel[modelType_].pMaterial != NULL)
	{
		this->xmodel[modelType_].pMaterial->Release();
		this->xmodel[modelType_].pMaterial = NULL;
	}
	for (unsigned int n = 0; n < xmodel[modelType_].nMaterialNum; n++)
	{
		this->xmodel[modelType_].pMeshTextures[n]->Release();
		this->xmodel[modelType_].pMeshTextures[n] = NULL;
	}
	delete[] xmodel[modelType_].pMeshTextures;
	xmodel[modelType_].pMeshTextures = NULL;
}
void XModel::Init()
{
	
}
void XModel::Uninit()
{
	
}
void XModel::Update()
{
	if (transform_.rotate.x > 360.0f){
		transform_.rotate.x = transform_.rotate.x - 360.0f;
	}
	if (transform_.rotate.x < -360.0f){
		transform_.rotate.x = transform_.rotate.x + 360.0f;
	}
	if (transform_.rotate.y > 360.0f) {
		transform_.rotate.y = transform_.rotate.y - 360.0f;
	}
	if (transform_.rotate.y < -360.0f) {
		transform_.rotate.y = transform_.rotate.y + 360.0f;
	}
	if (transform_.rotate.z > 360.0f) {
		transform_.rotate.z = transform_.rotate.z - 360.0f;
	}
	if (transform_.rotate.z < -360.0f) {
		transform_.rotate.z = transform_.rotate.z + 360.0f;
	}
}
void XModel::BeginDraw()
{
	if (!hieral_)
	{
		D3DXMATRIX mtxTrans, mtxScale, mtxRotateX, mtxRotateY, mtxRotateZ;
		D3DXMatrixIdentity(&world_);
		D3DXMatrixTranslation(&mtxTrans, Object::transform_.pos.x, Object::transform_.pos.y, Object::transform_.pos.z);
		D3DXMatrixScaling(&mtxScale, Object::transform_.scale.x, Object::transform_.scale.y, Object::transform_.scale.z);
		D3DXMatrixRotationX(&mtxRotateX, D3DXToRadian(Object::transform_.rotate.x));
		D3DXMatrixRotationY(&mtxRotateY, D3DXToRadian(Object::transform_.rotate.y));
		D3DXMatrixRotationZ(&mtxRotateZ, D3DXToRadian(Object::transform_.rotate.z));

		world_ = mtxScale;
		world_ *= mtxRotateX;
		world_ *= mtxRotateY;
		world_ *= mtxRotateZ;
		world_ *= mtxTrans;
	}

}
void XModel::EndDraw()
{
}
void XModel::SetWorld(D3DXMATRIX world)
{
	world_ = world;
}	
LPD3DXMESH XModel::GetMesh(XMODEL modelType)
{
	return xmodel[modelType].pMesh;
}
void XModel::SetHieral(bool flag)
{
	hieral_ = flag;
}
void XModel::Draw()
{
	if (!useShader_)
	{
		LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

		//FVFの設定
		pDevice->SetFVF(this->xmodel[this->modelType_].pMesh->GetFVF());

		//ライティング
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		//各種行列の設定
		pDevice->SetTransform(D3DTS_WORLD, &world_);

		//マテリアルの取得
		LPD3DXMATERIAL pMaterial = (LPD3DXMATERIAL)this->xmodel[this->modelType_].pMaterial->GetBufferPointer();

		//前面モデル描画
		for (unsigned int i = 0; i < xmodel[this->modelType_].nMaterialNum; i++)
		{
			pDevice->SetMaterial(&pMaterial[i].MatD3D);
			pDevice->SetTexture(0, xmodel[this->modelType_].pMeshTextures[i]);
			this->xmodel[this->modelType_].pMesh->DrawSubset(i);
		}
	}
	
	
}
void XModel::Draw(LPD3DXEFFECT effect,UINT pass)
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	//FVFの設定
	pDevice->SetFVF(this->xmodel[this->modelType_].pMesh->GetFVF());

	//ライティング
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//各種行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &world_);

	//マテリアルの取得
	LPD3DXMATERIAL pMaterial = (LPD3DXMATERIAL)this->xmodel[this->modelType_].pMaterial->GetBufferPointer();

	effect->Begin(NULL,0);
	effect->BeginPass(pass);

	//前面モデル描画
	for (unsigned int i = 0; i < xmodel[this->modelType_].nMaterialNum; i++)
	{
		D3DXVECTOR4 diffuse = D3DXVECTOR4(pMaterial[i].MatD3D.Diffuse.r, pMaterial[i].MatD3D.Diffuse.g, pMaterial[i].MatD3D.Diffuse.b, pMaterial[i].MatD3D.Diffuse.a);
		effect->SetVector("Diffuse", &diffuse);
		effect->SetTexture("MeshTex", xmodel[this->modelType_].pMeshTextures[i]);
		effect->CommitChanges();
		this->xmodel[this->modelType_].pMesh->DrawSubset(i);
	}

	effect->EndPass();
	effect->End();
}
bool XModel::LoadXFile(XModel::XMODEL modelType)
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	HRESULT hr;

	
	this->xmodel[modelType].nMaterialNum = 0;
	this->xmodel[modelType].pMaterial = NULL;
	this->xmodel[modelType].pMesh = NULL;

	LPD3DXBUFFER pAdjacency;

	hr = D3DXLoadMeshFromXA(
		g_aXFileName[modelType].fileName,				//ファイル名
		D3DXMESH_SYSTEMMEM,								//メッシュの設定ﾌﾗｸﾞ
		pDevice,										//メッシュに関連づけるデバイス
		&pAdjacency,									//隣接データを含むバッファを受け取る
		&this->xmodel[modelType].pMaterial,			//マテリアルデータを受け取る
		NULL,											//エフェクトの初期化に使う状態情報
		&this->xmodel[modelType].nMaterialNum,		//マテリアル数を受け取る
		&this->xmodel[modelType].pMesh				//読み込んだメッシュを受け取る変数へのポインタ
	);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Xmodel読み込み失敗", "警告", MB_OK | MB_ICONHAND);
		return false;
	}
	this->xmodel[modelType].pMeshTextures = new LPDIRECT3DTEXTURE9[this->xmodel[modelType].nMaterialNum];		// テクスチャ配列　メモリ動的確保

	for (unsigned int i = 0; i < this->xmodel[modelType].nMaterialNum; i++)
	{
		this->xmodel[modelType].pMeshTextures[i] = {};
	}

	//マテリアルの情報バッファポインタ
	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)this->xmodel[modelType].pMaterial->GetBufferPointer();

	//各マテリアル情報の保存
	for (unsigned long i = 0; i < this->xmodel[modelType].nMaterialNum; i++)
	{

		//テクスチャを読み込む
		if (pMaterials[i].pTextureFilename != NULL)
		{
			hr = D3DXCreateTextureFromFileA(pDevice, pMaterials[i].pTextureFilename, &this->xmodel[modelType].pMeshTextures[i]);
			if (FAILED(hr)) {
				this->xmodel[modelType].pMeshTextures[i] = NULL;
				MessageBoxA(NULL, "Xmodelテクスチャ読み込みに失敗しました。", "警告", MB_OK | MB_ICONHAND);
				D3DXCreateTextureFromFileA(pDevice, "resource/Xmodel/default.png", &this->xmodel[modelType].pMeshTextures[i]);
			}
		}
		else
		{
			this->xmodel[modelType].pMeshTextures[i] = NULL;
			D3DXCreateTextureFromFileA(pDevice, "resource/Xmodel/default.png", &this->xmodel[modelType].pMeshTextures[i]);
		}
	}



	LPD3DXMESH pTempMesh;
	hr = this->xmodel[modelType].pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL
	);



	if (pAdjacency != NULL)
	{
		pAdjacency->Release();
	}

	D3DVERTEXELEMENT9 Elements[MAXD3DDECLLENGTH + 1];
	this->xmodel[modelType].pMesh->GetDeclaration(Elements);

	hr = this->xmodel[modelType].pMesh->CloneMesh(
		D3DXMESH_MANAGED | D3DXMESH_WRITEONLY | D3DXMESH_32BIT,
		Elements,
		pDevice,
		&pTempMesh
	);

	if (this->xmodel[modelType].pMesh != NULL)
	{
		this->xmodel[modelType].pMesh->Release();
		this->xmodel[modelType].pMesh = pTempMesh;
	}
	


	return true;
}