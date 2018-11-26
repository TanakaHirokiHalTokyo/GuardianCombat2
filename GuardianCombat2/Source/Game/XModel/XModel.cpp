

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
	if (xmodel[modelType_].normalmapTexture)
	{
		xmodel[modelType_].normalmapTexture->Release();
		xmodel[modelType_].normalmapTexture = NULL;
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
	if (GetVisible())
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
bool XModel::AddTangentSpace()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	LPD3DXMESH		tempMesh;
	LPD3DXMESH		outMesh;
	HRESULT			hr;

	const D3DVERTEXELEMENT9 Decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },				// 位置情報
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },				// テクスチャ座標
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },				// 法線
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },				// 接ベクトル
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },				// 従法線ベクトル
		D3DDECL_END()
	};

	// 指定フォーマットのクローンメッシュを作る
	hr = xmodel[modelType_].pMesh->CloneMesh(xmodel[modelType_].pMesh->GetOptions(), Decl, pDevice, &tempMesh);
	if (FAILED(hr)) {
		MessageBox(NULL, "ERROR!!", "CloneMeshエラー", MB_OK);
		return false;
	}
	// 法線ベクトルを計算
	hr = D3DXComputeNormals(tempMesh, NULL);
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeNormalsエラー", MB_OK);
		return false;
	}

	// 従法線、接ベクトルをもつメッシュを作成する
	hr = D3DXComputeTangentFrameEx(tempMesh,
		D3DDECLUSAGE_TEXCOORD, 0,		// 0番目のテクスチャ座標を使用する 
		D3DDECLUSAGE_TANGENT, 0,		// 0番目の接ベクトルを計算する
		D3DDECLUSAGE_BINORMAL, 0,		// 0番目の従法線ベクトルを計算する
		D3DDECLUSAGE_NORMAL, 0,			// 0番目の法線ベクトルを計算する
		0,								// 計算のオプション
		NULL,							// 
		-1.01f,							// 隣接する３角形の閾値
		-0.01f,							// 単独とみなされる頂点の閾値
		-1.01f,							// 法線の閾値
		&outMesh,						// 生成されたメッシュ
		NULL);							//  
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeTangentFrameExエラー", MB_OK);
		return false;
	}

	xmodel[modelType_].pMesh->Release();
	xmodel[modelType_].pMesh = outMesh;

	return true;
}
bool XModel::LoadNormalmapTexture(const char * filename)
{
	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	hr = D3DXCreateTextureFromFile(pDevice,
		filename,
		&xmodel[modelType_].normalmapTexture);
	if (FAILED(hr)) {
			return false;
	}

	return true;
}
void XModel::SetHieral(bool flag)
{
	hieral_ = flag;
}
void XModel::Draw()
{
	if (GetVisible())
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
}
void XModel::Draw(LPD3DXEFFECT effect,UINT pass)
{
	if (GetVisible())
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

		effect->Begin(NULL, 0);
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
		g_aXFileName[modelType].fileName,						//ファイル名
		D3DXMESH_SYSTEMMEM,									//メッシュの設定ﾌﾗｸﾞ
		pDevice,																	//メッシュに関連づけるデバイス
		&pAdjacency,															//隣接データを含むバッファを受け取る
		&this->xmodel[modelType].pMaterial,					//マテリアルデータを受け取る
		NULL,																		//エフェクトの初期化に使う状態情報
		&this->xmodel[modelType].nMaterialNum,			//マテリアル数を受け取る
		&this->xmodel[modelType].pMesh						//読み込んだメッシュを受け取る変数へのポインタ
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