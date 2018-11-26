

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
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },				// �ʒu���
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },				// �e�N�X�`�����W
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },				// �@��
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },				// �ڃx�N�g��
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },				// �]�@���x�N�g��
		D3DDECL_END()
	};

	// �w��t�H�[�}�b�g�̃N���[�����b�V�������
	hr = xmodel[modelType_].pMesh->CloneMesh(xmodel[modelType_].pMesh->GetOptions(), Decl, pDevice, &tempMesh);
	if (FAILED(hr)) {
		MessageBox(NULL, "ERROR!!", "CloneMesh�G���[", MB_OK);
		return false;
	}
	// �@���x�N�g�����v�Z
	hr = D3DXComputeNormals(tempMesh, NULL);
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeNormals�G���[", MB_OK);
		return false;
	}

	// �]�@���A�ڃx�N�g���������b�V�����쐬����
	hr = D3DXComputeTangentFrameEx(tempMesh,
		D3DDECLUSAGE_TEXCOORD, 0,		// 0�Ԗڂ̃e�N�X�`�����W���g�p���� 
		D3DDECLUSAGE_TANGENT, 0,		// 0�Ԗڂ̐ڃx�N�g�����v�Z����
		D3DDECLUSAGE_BINORMAL, 0,		// 0�Ԗڂ̏]�@���x�N�g�����v�Z����
		D3DDECLUSAGE_NORMAL, 0,			// 0�Ԗڂ̖@���x�N�g�����v�Z����
		0,								// �v�Z�̃I�v�V����
		NULL,							// 
		-1.01f,							// �אڂ���R�p�`��臒l
		-0.01f,							// �P�ƂƂ݂Ȃ���钸�_��臒l
		-1.01f,							// �@����臒l
		&outMesh,						// �������ꂽ���b�V��
		NULL);							//  
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeTangentFrameEx�G���[", MB_OK);
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

			//FVF�̐ݒ�
			pDevice->SetFVF(this->xmodel[this->modelType_].pMesh->GetFVF());

			//���C�e�B���O
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			//�e��s��̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &world_);

			//�}�e���A���̎擾
			LPD3DXMATERIAL pMaterial = (LPD3DXMATERIAL)this->xmodel[this->modelType_].pMaterial->GetBufferPointer();

			//�O�ʃ��f���`��
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

		//FVF�̐ݒ�
		pDevice->SetFVF(this->xmodel[this->modelType_].pMesh->GetFVF());

		//���C�e�B���O
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		//�e��s��̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &world_);

		//�}�e���A���̎擾
		LPD3DXMATERIAL pMaterial = (LPD3DXMATERIAL)this->xmodel[this->modelType_].pMaterial->GetBufferPointer();

		effect->Begin(NULL, 0);
		effect->BeginPass(pass);

		//�O�ʃ��f���`��
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
		g_aXFileName[modelType].fileName,						//�t�@�C����
		D3DXMESH_SYSTEMMEM,									//���b�V���̐ݒ��׸�
		pDevice,																	//���b�V���Ɋ֘A�Â���f�o�C�X
		&pAdjacency,															//�אڃf�[�^���܂ރo�b�t�@���󂯎��
		&this->xmodel[modelType].pMaterial,					//�}�e���A���f�[�^���󂯎��
		NULL,																		//�G�t�F�N�g�̏������Ɏg����ԏ��
		&this->xmodel[modelType].nMaterialNum,			//�}�e���A�������󂯎��
		&this->xmodel[modelType].pMesh						//�ǂݍ��񂾃��b�V�����󂯎��ϐ��ւ̃|�C���^
	);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Xmodel�ǂݍ��ݎ��s", "�x��", MB_OK | MB_ICONHAND);
		return false;
	}
	this->xmodel[modelType].pMeshTextures = new LPDIRECT3DTEXTURE9[this->xmodel[modelType].nMaterialNum];		// �e�N�X�`���z��@���������I�m��

	for (unsigned int i = 0; i < this->xmodel[modelType].nMaterialNum; i++)
	{
		this->xmodel[modelType].pMeshTextures[i] = {};
	}

	//�}�e���A���̏��o�b�t�@�|�C���^
	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)this->xmodel[modelType].pMaterial->GetBufferPointer();

	//�e�}�e���A�����̕ۑ�
	for (unsigned long i = 0; i < this->xmodel[modelType].nMaterialNum; i++)
	{

		//�e�N�X�`����ǂݍ���
		if (pMaterials[i].pTextureFilename != NULL)
		{
			hr = D3DXCreateTextureFromFileA(pDevice, pMaterials[i].pTextureFilename, &this->xmodel[modelType].pMeshTextures[i]);
			if (FAILED(hr)) {
				this->xmodel[modelType].pMeshTextures[i] = NULL;
				MessageBoxA(NULL, "Xmodel�e�N�X�`���ǂݍ��݂Ɏ��s���܂����B", "�x��", MB_OK | MB_ICONHAND);
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