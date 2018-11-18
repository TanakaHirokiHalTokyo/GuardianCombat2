
#include <iostream>
#include <fstream>
#include <string>
#include "MeshField.h"
#include "../../DirectXRenderer.h"
#include "../Object.h"
#include "../../Imgui/ImguiManager.h"
#include "../Shader/ShadowMapShader.h"
#include "../Shader/ShadowVolume.h"

constexpr bool USE_SHADER = true;

int MeshField::spliteX_ = 0;
int MeshField::spliteY_ = 0;
float MeshField::sizeX_ = 0.0f;
float MeshField::sizeY_ = 0.0f;
MeshField::VERTEX3D* MeshField::Vertex = nullptr;
WORD*		MeshField::pIndex = nullptr;

MeshField::MeshField()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	HRESULT hr;


	hr = D3DXCreateTextureFromFileA(pDevice, "resource/texture/ground.jpg", &this->pTexture_);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "テクスチャ読み込み失敗", "警告", MB_OK | MB_ICONHAND);
	}


	sizeX_ = 80.0f;
	sizeY_ = 80.0f;
	spliteX_ = 40;
	spliteY_ = 40;
	type_ = MeshField::Field_Ground;

	useShader_ = true;
}
MeshField::~MeshField()
{

	if (meshf_[type_].iBuf != NULL)
	{
		meshf_[type_].iBuf->Release();
		meshf_[type_].iBuf = NULL;
	}
	if (meshf_[type_].vBuf != NULL)
	{
		meshf_[type_].vBuf->Release();
		meshf_[type_].vBuf = NULL;
	}

	if (this->pTexture_ != NULL)
	{
		this->pTexture_->Release();
		this->pTexture_ = NULL;
	}
	
}
void MeshField::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	ZeroMemory(&this->mat_, sizeof(this->mat_));
	mat_.Diffuse.r = 0.1f;
	mat_.Diffuse.g = 1.0f;
	mat_.Diffuse.b = 0.1f;
	mat_.Diffuse.a = 1.0f;

	mat_.Ambient.r = 0.3f;
	mat_.Ambient.g = 0.3f;
	mat_.Ambient.b = 0.3f;
	mat_.Ambient.a = 1.0f;

	std::ifstream file;
	file.open("resource/FieldData.bin", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "地形データを読み込めませんでした。\nデフォルトデータを使用します。", "警告", MB_OK | MB_ICONHAND);
		this->SetMeshField(sizeX_, sizeY_, spliteX_, spliteY_, type_);
	}
	else
	{
		file.read((char*)&spliteX_, sizeof(int));
		file.read((char*)&spliteY_, sizeof(int));
		file.read((char*)&sizeX_, sizeof(float));
		file.read((char*)&sizeY_, sizeof(float));
		file.read((char*)&meshf_[type_].vNum, sizeof(int));

		//VertexBuffer作成
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX3D) * meshf_[type_].vNum,		//頂点数分のサイズ
			D3DUSAGE_WRITEONLY,								//書き込みしかしない
			FVF_VERTEX3D,									//FVF
			D3DPOOL_MANAGED,								//デバイスが失われたとき勝手に復旧する
			&meshf_[type_].vBuf,								//頂点ばっふぁを管理するためのアドレス	使わなくなったらRelease
			NULL
		);

		meshf_[type_].vBuf->Lock(
			0,
			0,
			(void**)&Vertex,
			D3DLOCK_DISCARD
		);
		for (int i = 0; i < meshf_[type_].vNum; i++)
		{

			file.read((char*)&Vertex[i], sizeof(VERTEX3D));

		}
		meshf_[type_].vBuf->Unlock();

		file.read((char*)&meshf_[type_].iNum, sizeof(int));

		//Indexバッファ作成
		pDevice->CreateIndexBuffer(
			sizeof(WORD) * meshf_[type_].iNum,		//インデックスのサイズ
			D3DUSAGE_WRITEONLY,						//書き込みしかしない
			D3DFMT_INDEX16,							//(indexの型 今WORD(unsigned short)のため
			D3DPOOL_MANAGED,						//デバイスが失われたとき勝手に復旧する
			&meshf_[type_].iBuf,						//インデックスばっふぁを管理するためのアドレス	使わなくなったらRelease
			NULL
		);


		meshf_[type_].iBuf->Lock(
			0,
			0,
			(void**)&pIndex,
			D3DLOCK_DISCARD
		);

		for (int i = 0; i < meshf_[type_].iNum; i++)
		{
			file.read((char*)&pIndex[i], sizeof(WORD));

		}

		meshf_[type_].iBuf->Unlock();
	}

	useShader_ = USE_SHADER;
	SetScale(1.0f, 1.0f, 1.0f);

	file.close();
}
void MeshField::Uninit()
{

}
void MeshField::Update()
{
	sizeX_ = sizeX_;
	sizeY_ = sizeY_;
}
void MeshField::BeginDraw()
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

	if (useShader_)
	{
		LPD3DXEFFECT effect;
		ShadowMapShader::SetInfo(world_);
		effect = ShadowMapShader::GetEffect();
		effect->SetTechnique("Shadow");
		Draw(effect, 0);
	}
}
void MeshField::Draw()
{
	if (!useShader_)
	{
		LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

		// GPUとVertexBufferをパイプラインでつなぐ - +
		pDevice->SetStreamSource(
			0,						//パイプラインのセット番号
			meshf_[type_].vBuf,		//バッファ
			0,						//どこから書き込むか
			sizeof(VERTEX3D)		//隣の頂点までどれくらいの長さ
		);
		pDevice->SetIndices(
			meshf_[type_].iBuf				//バッファ
		);

		//FVFの設定
		pDevice->SetFVF(FVF_VERTEX3D);

		//ライトオンにする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetTransform(D3DTS_WORLD, &world_);
		pDevice->SetMaterial(&mat_);
		pDevice->SetTexture(0, this->pTexture_);

		//次からバッファ使いたくないときが来たときバッファをNULLにすれば使わなくなる。 

		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			meshf_[type_].vNum,			//頂点数
			0,
			meshf_[type_].iNum - 2		//プリミティブ数
		);
	}
	else
	{

		LPD3DXEFFECT effect;

		ShadowMapShader::SetInfo(world_);
		effect = ShadowMapShader::GetEffect();
		effect->SetTechnique("Scene");
		Draw(effect, 0);

		
	}
}
void MeshField::Draw(LPD3DXEFFECT effect, UINT pass)
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

	// GPUとVertexBufferをパイプラインでつなぐ - +
	pDevice->SetStreamSource(
		0,						//パイプラインのセット番号
		meshf_[type_].vBuf,		//バッファ
		0,						//どこから書き込むか
		sizeof(VERTEX3D)		//隣の頂点までどれくらいの長さ
	);
	pDevice->SetIndices(
		meshf_[type_].iBuf				//バッファ
	);

	//FVFの設定
	pDevice->SetFVF(FVF_VERTEX3D);

	//ライトオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetTransform(D3DTS_WORLD, &world_);

	effect->Begin(NULL, 0);
	effect->BeginPass(pass);

	D3DXVECTOR4 diffuse = D3DXVECTOR4(mat_.Diffuse.r, mat_.Diffuse.g, mat_.Diffuse.b, mat_.Diffuse.a);
	D3DXVECTOR4 ambient = D3DXVECTOR4(mat_.Ambient.r, mat_.Ambient.g, mat_.Ambient.b, mat_.Ambient.a);
	effect->SetVector("Diffuse", &diffuse);
	effect->SetVector("Ambient", &ambient);
	effect->SetTexture("MeshTex", this->pTexture_);

	effect->CommitChanges();

	//次からバッファ使いたくないときが来たときバッファをNULLにすれば使わなくなる。

	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		meshf_[type_].vNum,			//頂点数
		0,
		meshf_[type_].iNum - 2		//プリミティブ数
	);

	

	effect->EndPass();
	effect->End();

	
}
void MeshField::EndDraw()
{
}
float MeshField::GetSizeX()
{
	return sizeX_;
}
float MeshField::GetSizeY()
{
	return sizeY_;
}
float MeshField::GetSpliteX()
{
	return (float)spliteX_;
}
float MeshField::GetSpliteY()
{
	return (float)spliteY_;
}
void MeshField::SetMeshField(float SizeX, float SizeY, int SpliteX, int SpliteY, MeshField::MeshField_Type type)
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	
	

	meshf_[type].vNum = (SpliteX + 1) * (SpliteY + 1);
	meshf_[type].iNum = (((SpliteX + 1) * 2) * SpliteY) + ((SpliteY - 1) * 2);

	//VertexBuffer作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX3D) * meshf_[type].vNum,		//頂点数分のサイズ
		D3DUSAGE_WRITEONLY,								//書き込みしかしない
		FVF_VERTEX3D,									//FVF
		D3DPOOL_MANAGED,								//デバイスが失われたとき勝手に復旧する
		&meshf_[type].vBuf,								//頂点ばっふぁを管理するためのアドレス	使わなくなったらRelease
		NULL
	);


	meshf_[type].vBuf->Lock(
		0,
		0,
		(void**)&Vertex,
		D3DLOCK_DISCARD						//書き込んだら開けない　速度重視	Unlockしたらデータは失われる
	);

	if (type == Field_Ground)
	{
		for (LONG i = 0; i < meshf_[type].vNum; i++)
		{
			Vertex[i].pos = D3DXVECTOR3((float)(-SizeX * SpliteX / 2) + (SizeX * (i % (SpliteX + 1))),0.0f, (float)(SizeY * SpliteY / 2) - (SizeY * (i / (SpliteX + 1))));
			Vertex[i].texcoord = D3DXVECTOR2((FLOAT)0.0f + 1.0f * (i % (SpliteX + 1)), (FLOAT)0.0f + 1.0f * (i / (SpliteX + 1)));
		}

		for (LONG i = 0; i < meshf_[type].vNum; i++)
		{
			Vertex[i].color = D3DCOLOR_RGBA(0, 255, 0, 255);
			Vertex[i].vec = D3DXVECTOR3(0, 1.0f, 0);
		}
		for (LONG i = 1; i < meshf_[type].vNum -1; i++)
		{
			
			D3DXVECTOR3 vx,vz, nx, nz, n;
			vx = Vertex[i + 1].pos - Vertex[i - 1].pos;
			vz = Vertex[i - (spliteX_ + 1)].pos - Vertex[i + spliteX_ + 1].pos;
			
			nx.x = -vx.y;
			nx.y = vx.x;
			nx.z = 0.0f;

			nz.x = 0.0f;
			nz.y = vz.z;
			nz.z = -vz.y;

			n = nx + nz;
			D3DXVec3Normalize(&n, &n);
			Vertex[i].vec = n;
		}
	}

	else if (type == Field_Wall)
	{
		for (LONG i = 0; i < meshf_[type].vNum; i++)
		{
			Vertex[i].pos = D3DXVECTOR3((float)(-SizeX * SpliteX / 2) + (SizeX * (i % (SpliteX + 1))), (float)(SizeY * SpliteY) - (SizeY * (i / (SpliteX + 1))), 0.0f);
			Vertex[i].texcoord = D3DXVECTOR2((FLOAT)0.0f + 1.0f * (i % (SpliteX + 1)), (FLOAT)0.0f + 1.0f * (i / (SpliteX + 1)));
		}
		for (LONG i = 0; i < meshf_[type].vNum; i++)
		{
			Vertex[i].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			Vertex[i].vec = D3DXVECTOR3(0, 0.5f, 0);
		}
	}
	
	meshf_[type].vBuf->Unlock();


	//Indexバッファ作成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * meshf_[type].iNum,		//インデックスのサイズ
		D3DUSAGE_WRITEONLY,						//書き込みしかしない
		D3DFMT_INDEX16,							//(indexの型 今WORD(unsigned short)のため
		D3DPOOL_MANAGED,						//デバイスが失われたとき勝手に復旧する
		&meshf_[type].iBuf,						//インデックスばっふぁを管理するためのアドレス	使わなくなったらRelease
		NULL
	);

	meshf_[type].iBuf->Lock(
		0,
		0,
		(void**)&pIndex,
		D3DLOCK_DISCARD
	);

	int StartIndex = SpliteX + 1;
	int DegenerateCount = 0;
	bool DegenerateFlag = false;		//縮退した直後にtrue
	bool Degenerate = false;			//縮退を行うかのFLAG
	for (int i = 0; i < meshf_[type].iNum; i += 2)
	{
		//縮退するとき
		if (Degenerate == true)
		{
			pIndex[i] = pIndex[i - 1];

			pIndex[i + 1] = pIndex[i - 2] + 1;

			Degenerate = false;
			DegenerateFlag = true;
		}
		//通常時の処理
		else if (DegenerateFlag == false)
		{
			if (i == 0) { pIndex[i] = (WORD)StartIndex; }

			else { pIndex[i] = pIndex[i - 2] + 1; }

			pIndex[i + 1] = (WORD)pIndex[i] - StartIndex;

			DegenerateCount++;
			if (DegenerateCount % StartIndex == 0)
			{
				Degenerate = true;
			}
		}
		//縮退した直後
		else if (DegenerateFlag == true)
		{
			pIndex[i] = pIndex[i - 4] + 1;

			pIndex[i + 1] = (WORD)pIndex[i] - StartIndex;

			DegenerateCount++;
			if (DegenerateCount % StartIndex == 0)
			{
				Degenerate = true;
			}
			DegenerateFlag = false;
		}
	}

	meshf_[type].iBuf->Unlock();
}
float MeshField::GetHeight(D3DXVECTOR3 position)
{
	//面番号調整変数
	int offsetValue = 0;
	//頂点要素番号
	int p0 = spliteX_, p1 = -1, p2 = 0, p3 = spliteX_+1;

	//どこの面の上に立っているかを調べる。
	for (int j = 0; j < spliteY_; j++)
	{
		for (int i = 0; i < spliteX_; i++)
		{
			//頂点要素番号更新
			p0 = p0 + 1 + offsetValue;
			p1 = p1 + 1 + offsetValue;
			p2 = p2 + 1 + offsetValue;
			p3 = p3 + 1 + offsetValue;

			offsetValue = 0;

			//頂点からプレイヤーに向かうベクトルを求める。
			D3DXVECTOR3 v0p = position - Vertex[p0].pos;
			D3DXVECTOR3 v1p = position - Vertex[p1].pos;
			D3DXVECTOR3 v2p = position - Vertex[p2].pos;
			D3DXVECTOR3 v3p = position - Vertex[p3].pos;


			//面側の向きベクトルを求める。
			D3DXVECTOR3 v01 = Vertex[p1].pos - Vertex[p0].pos;
			D3DXVECTOR3 v13 = Vertex[p3].pos - Vertex[p1].pos;
			D3DXVECTOR3 v30 = Vertex[p0].pos - Vertex[p3].pos;


			//結果を求める。
			float c0 = v01.x * v0p.z - v01.z * v0p.x;
			float c1 = v13.x * v1p.z - v13.z * v1p.x;
			float c2 = v30.x * v3p.z - v30.z * v3p.x;

			if (c0 <= 0.0f && c1 <= 0.0f && c2 <= 0.0f)
			{
				D3DXVECTOR3 n = D3DXVECTOR3(0,0,0);
				D3DXVec3Normalize(&v01, &v01);
				D3DXVec3Normalize(&v13, &v13);

				D3DXVec3Cross(&n,&v01,&v13);
				float y = Vertex[p0].pos.y - ((n.x * (position.x - Vertex[p0].pos.x) + n.z * (position.z - Vertex[p0].pos.z)) / n.y);
				return y;
			}

			//面の向きベクトルを求める
			D3DXVECTOR3 v32 = Vertex[p2].pos - Vertex[p3].pos;
			D3DXVECTOR3 v21 = Vertex[p1].pos - Vertex[p2].pos;
			//D3DXVECTOR3 v13 = Vertex[p3].pos - Vertex[p1].pos;

			D3DXVec3Normalize(&v32, &v32);
			D3DXVec3Normalize(&v21, &v21);

			//外積結果を求める。
			c0 = v32.x * v3p.z - v32.z * v3p.x;
			c1 = v21.x * v2p.z - v21.z * v2p.x;
			c2 = v13.x * v1p.z - v13.z * v1p.x;

			if (c0 >= 0.0f && c1 >= 0.0f && c2 >= 0.0f)
			{
				D3DXVECTOR3 n = D3DXVECTOR3(0,0,0);
				D3DXVec3Normalize(&v32, &v32);
				D3DXVec3Normalize(&v21, &v21);

				D3DXVec3Cross(&n, &v32, &v21);
				float y = Vertex[p3].pos.y - ((n.x * (position.x - Vertex[p3].pos.x) + n.z * (position.z - Vertex[p3].pos.z)) / n.y);

				return y;
			}

		}
		//面番号改行するときに加算する。
		offsetValue += 1;
	}
	return 0.0f;
}