
#include "AnimPlayer.h"
#include "../XModel/XModel.h"
#include "../Shader/ShadowMapShader.h"
#include "../../DirectXRenderer.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ToonShader.h"
#include "../../DInput/DirectInput.h"
#include "../../Imgui/ImguiManager.h"

AnimPlayer::AnimPlayer()
{
	parts[0].ModelInfo = new XModel();
	parts[0].ModelInfo->Init();
	parts[0].ModelInfo->SetModelType(XModel::MODEL_BODY);

	parts[1].ModelInfo = new XModel();
	parts[1].ModelInfo->Init();
	parts[1].ModelInfo->SetModelType(XModel::MODEL_HEAD);

	parts[4].ModelInfo = new XModel();
	parts[4].ModelInfo->Init();
	parts[4].ModelInfo->SetModelType(XModel::MODEL_ARM_L);

	parts[2].ModelInfo = new XModel();
	parts[2].ModelInfo->Init();
	parts[2].ModelInfo->SetModelType(XModel::MODEL_ARM_R);

	parts[5].ModelInfo = new XModel();
	parts[5].ModelInfo->Init();
	parts[5].ModelInfo->SetModelType(XModel::MODEL_ARM_L);

	parts[3].ModelInfo = new XModel();
	parts[3].ModelInfo->Init();
	parts[3].ModelInfo->SetModelType(XModel::MODEL_ARM_R);

	parts[8].ModelInfo = new XModel();
	parts[8].ModelInfo->Init();
	parts[8].ModelInfo->SetModelType(XModel::MODEL_ASI);

	parts[6].ModelInfo = new XModel();
	parts[6].ModelInfo->Init();
	parts[6].ModelInfo->SetModelType(XModel::MODEL_ASI);

	parts[9].ModelInfo = new XModel();
	parts[9].ModelInfo->Init();
	parts[9].ModelInfo->SetModelType(XModel::MODEL_ASI);

	parts[7].ModelInfo = new XModel();
	parts[7].ModelInfo->Init();
	parts[7].ModelInfo->SetModelType(XModel::MODEL_ASI);

	//親子関係
	parts[BODY].Parent = NULL;
	parts[HEAD].Parent = &parts[0];
	parts[ARM_L].Parent = &parts[0];
	parts[HAND_L].Parent = &parts[2];
	parts[ARM_R].Parent = &parts[0];
	parts[HAND_R].Parent = &parts[4];
	parts[ASI_L].Parent = &parts[0];
	parts[ASIKUBI_L].Parent = &parts[6];
	parts[ASI_R].Parent = &parts[0];
	parts[ASIKUBI_R].Parent = &parts[8];
	
	//相対座標セット
	parts[BODY].ModelInfo->SetPosition(0.0f, 1.0f,0.0f);
	parts[HEAD].ModelInfo->SetPosition(0.0f, 5.0f,0.0f);
	parts[ARM_L].ModelInfo->SetPosition(-1.0f, 0.0f,0.0f);
	parts[HAND_L].ModelInfo->SetPosition(-1.0f,0.0f,0.0f);
	parts[ARM_R].ModelInfo->SetPosition(1.0f, 0.0f,0.0f);
	parts[HAND_R].ModelInfo->SetPosition(1.0f,0.0f, 0.0f);
	parts[ASI_L].ModelInfo->SetPosition(-1.0f,-1.0f,0.0f);
	parts[ASIKUBI_L].ModelInfo->SetPosition(0.0f,-1.0f, 0.0f);
	parts[ASI_R].ModelInfo->SetPosition(1.0f,-1.0f,0.0f);
	parts[ASIKUBI_R].ModelInfo->SetPosition(0.0f,-1.0f,0.0f);

	bool fileopen = false;
	bool fileAnim = false;

	

	for (int AnimTypeCount = 0; AnimTypeCount < ANIMTYPE_MAX; AnimTypeCount++)
	{
		std::ifstream fileTrans;

		fileTrans.open("AnimTransData.bin", std::ios::binary | std::ios::in);
		if (fileTrans.fail())
		{
			MessageBoxA(NULL, "アニメーションデータを読み込めませんでした。\nデフォルトデータを使用します。", "警告", MB_OK | MB_ICONHAND);
		}
		else
		{
			fileopen = true;
		}
		std::ifstream file_anim;
		file_anim.open(animfile[AnimTypeCount].FileName, std::ios::binary | std::ios::in);
		if (file_anim.fail())
		{
			MessageBoxA(NULL, "アニメーションデータを読み込めませんでした。\nデフォルトデータを使用します。", "警告", MB_OK | MB_ICONHAND);
		}
		else
		{
			fileAnim = true;
		}
		for (unsigned int i = 0; i < PART_MAX; i++)
		{
			
			parts[i].ModelInfo->SetScale(0.2f, 0.2f, 0.2f);
			parts[i].ModelInfo->SetHieral(true);
			parts[i].ModelInfo->SetUseShader(true);

			if (fileopen)
			{
				D3DXVECTOR3 pos;
				D3DXVECTOR3 scale;
				D3DXVECTOR3 rot;
				fileTrans.read((char*)&pos.x, sizeof(float));
				fileTrans.read((char*)&pos.y, sizeof(float));
				fileTrans.read((char*)&pos.z, sizeof(float));
				fileTrans.read((char*)&scale.x, sizeof(float));
				fileTrans.read((char*)&scale.y, sizeof(float));
				fileTrans.read((char*)&scale.z, sizeof(float));
				fileTrans.read((char*)&rot.x, sizeof(float));
				fileTrans.read((char*)&rot.y, sizeof(float));
				fileTrans.read((char*)&rot.z, sizeof(float));

				parts[i].ModelInfo->SetRotation(rot);
				parts[i].ModelInfo->SetPosition(pos);
				parts[i].ModelInfo->SetScale(scale);
			}
			
			if (fileAnim)
			{
				file_anim.read((char*)&motion_[AnimTypeCount].keynum, sizeof(UINT));
				for (UINT keyCount = 0; keyCount < motion_[AnimTypeCount].keynum; keyCount++)
				{
					file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].frame, sizeof(int));
					for (int partCount = 0; partCount < PART_MAX; partCount++)
					{
						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].trans[partCount].rotate.x,sizeof(float));
						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].trans[partCount].rotate.y, sizeof(float));
						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].trans[partCount].rotate.z, sizeof(float));

						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].animRot[partCount].rightRot[0], sizeof(bool));
						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].animRot[partCount].rightRot[1], sizeof(bool));
						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].animRot[partCount].rightRot[2], sizeof(bool));

						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].animRot[partCount].min[0], sizeof(bool));
						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].animRot[partCount].min[1], sizeof(bool));
						file_anim.read((char*)&motion_[AnimTypeCount].animdata[keyCount].animRot[partCount].min[2], sizeof(bool));
					}
				}
			}
			fileAnim = false;
		}
		fileTrans.close();
		file_anim.close();
	}
	
	
}

AnimPlayer::~AnimPlayer()
{
	for (int i = 0; i < PART_MAX; i++)
	{
		parts[i].ModelInfo->Uninit();
		SAFE_DELETE(parts[i].ModelInfo);
	}
}

void AnimPlayer::Init()
{
	
	for (int i = 0; i < PART_MAX; i++)
	{
		animRotate_[i] = D3DXVECTOR3(0, 0, 0);
	}
	animPlaying_ = true;
}

void AnimPlayer::Uninit()
{
	
}

void AnimPlayer::Update()
{
	AnimationPlay();
	
	for (int i = 0; i < 10; i++)
	{
		parts[i].ModelInfo->Update();
	}

	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
	{
		if (!GetKeyboardPress(DIK_LSHIFT))
		{
			animCheck_.AnimNumber = WALK;
			if (animationType != WALK)
			{
				nowFrame = 0;
				nowAnimKey_ = 0;
				for (int i = 0; i < PART_MAX; i++)
				{
					parts[i].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[0].trans[i].rotate);
				}
			}
			animationType = WALK;
			walk = true;
		}
	}
	else
	{
		walk = false;
	}
	if (!walk)
	{
		animCheck_.AnimNumber = IDLE;
		if (animationType != IDLE)
		{
			nowFrame = 0;
			nowAnimKey_ = 0;
			for (int i = 0; i < PART_MAX; i++)
			{
				parts[i].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[0].trans[i].rotate);
			}
		}
		animationType = IDLE;
	}
	else
	{
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			animCheck_.AnimNumber = RUN;
			if (animationType != RUN)
			{
				nowFrame = 0;
				nowAnimKey_ = 0;
				for (int i = 0; i < PART_MAX; i++)
				{
					parts[i].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[0].trans[i].rotate);
				}
			}
			animationType = RUN;
		}
	}
	if (typeChanged_)
	{
		
	}

	/*if (!animCheck_.play)
	{
		this->UpdatePart();
	}
	*/
}

void AnimPlayer::BeginDraw()
{
	for (int i = 0; i < 10; i++)
	{
		const auto& model = parts[i].ModelInfo;
		auto World = parts[i].ModelInfo->GetWorld();
		D3DXMatrixIdentity(&World);
		D3DXMATRIX Rotation, Trans, Scale;
		D3DXMatrixIdentity(&Rotation);
		D3DXMatrixIdentity(&Trans);
		D3DXMatrixIdentity(&Scale);
		D3DXMatrixScaling(&Scale, 1.0f, 1.0f, 1.0f);
		D3DXMatrixMultiply(&World, &World, &Scale);
		D3DXMatrixRotationYawPitchRoll(&Rotation, D3DXToRadian(model->GetRotate().x), D3DXToRadian(model->GetRotate().y), D3DXToRadian(model->GetRotate().z));
		D3DXMatrixMultiply(&World, &World, &Rotation);
		D3DXMatrixTranslation(&Trans, model->GetPosition().x, model->GetPosition().y, model->GetPosition().z);
		D3DXMatrixMultiply(&World, &World, &Trans);

		if (parts[i].Parent != NULL)
		{
			D3DXMATRIX ParentMatrix = parts[i].Parent->Matrix;
			D3DXMatrixMultiply(&World, &World, &ParentMatrix);
		}
		
		parts[i].Matrix = World;

		*D3DXMatrixIdentity(&Scale);
		D3DXMatrixScaling(&Scale, model->GetScale().x, model->GetScale().y, model->GetScale().z);
		D3DXMatrixMultiply(&World, &World, &Scale);

		parts[i].ModelInfo->SetWorld(World);

		if (parts[i].ModelInfo->GetUseShader())
		{
			LPD3DXEFFECT effect;

			const auto& world = parts[i].ModelInfo->GetWorld();
			ShadowMapShader::SetInfo(world);
			effect = ShadowMapShader::GetEffect();
			effect->SetTechnique("Shadow");
			parts[i].ModelInfo->Draw(effect, 0);
		}
	}
}

void AnimPlayer::Draw()
{
	for (int i = 0; i < 10; i++)
	{
		if (parts[i].ModelInfo->GetUseShader())
		{
			//World座標
			const auto& world = parts[i].ModelInfo->GetWorld();
			LPD3DXEFFECT effect;

			ShadowMapShader::SetInfo(world);		//シャドウマップ　情報セット
			effect = ShadowMapShader::GetEffect();	//シャドウマップ　エフェクト取得
			effect->SetTechnique("Scene");			//シャドウマップ　シーン描画
			parts[i].ModelInfo->Draw(effect, 0);

			OutlineShader::SetInfo(world);			//アウトライン　情報セット
			effect = OutlineShader::GetEffect();	//アウトライン　エフェクト取得
			effect->SetTechnique("Tech");			//アウトライン描画
			parts[i].ModelInfo->Draw(effect, 0);

			ToonShader::SetInfo(world);				//トゥーン情報セット
			effect = ToonShader::GetEffect();		//トゥーン　エフェクト取得
			effect->SetTechnique("ToonPaint");		//Toon(Outline と　トゥーンシェーダー)
			parts[i].ModelInfo->Draw(effect, 0);

			
		}
		parts[i].ModelInfo->Draw();
	}
}

void AnimPlayer::EndDraw()
{

}

D3DXMATRIX AnimPlayer::GetMatrix(UINT Index)
{
	return parts[Index].Matrix;
}

void AnimPlayer::UpdatePart()
{	
	bool click = false;

	ImGui::Begin("Animations");
	ImGui::Text("KeyNum %d",motion_[animCheck_.AnimNumber].keynum);
	ImGui::Text("ViewKey %d",viewKey_);
	ImGui::Text("Animation Change : Up/Down");
	ImGui::Text("F2 : AnimationSave");
	ImGui::Text("F3 : TransformSave");
	ImGui::InputInt("Frame",&frame_);
	if (motion_[animCheck_.AnimNumber].keynum > 1)
	{
		if (GetKeyboardTrigger(DIK_SPACE))
		{
			if (!animPlaying_)
			{
				animPlaying_ = true;
				nowFrame = 0;
				nowAnimKey_ = 0;
				for (int i = 0; i < PART_MAX; i++)
				{
					parts[i].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[0].trans[i].rotate);
				}
			}
			else
			{
				animPlaying_ = false;
				viewKey_ = 0;
				frame_ = 0;
				nowAnimKey_ = 0;
				for (int partCount = 0; partCount < PART_MAX; partCount++)
				{
					parts[partCount].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[viewKey_].trans[partCount].rotate);
				}
			}
			
		}
		if (ImGui::Checkbox("Play Animation", &animPlaying_))
		{
			nowFrame = 0;
			nowAnimKey_ = 0;
			for (int i = 0; i < PART_MAX; i++)
			{
				parts[i].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[0].trans[i].rotate);
			}
		}
	}
	if (ImGui::RadioButton("Clear",click))
	{
		for (UINT i = 0; i < keyNum_; i++)
		{
			for (int partCount = 0; partCount < PART_MAX; partCount++)
			{
				motion_[animCheck_.AnimNumber].animdata[motion_[animCheck_.AnimNumber].keynum].trans[i].rotate= D3DXVECTOR3(0, 0, 0);
			}
		}
		click = false;
		keyNum_ = 0;
	}

	if (!animPlaying_)
	{
		if (GetKeyboardPress(DIK_F3))
		{
			std::ofstream fileTrans;
			fileTrans.open("AnimTransData.bin", std::ios::binary | std::ios::out);

			for (int i = 0; i < PART_MAX; i++)
			{
				const auto& pos = parts[i].ModelInfo->GetPosition();
				const auto& scale = parts[i].ModelInfo->GetScale();
				const auto& rot = parts[i].ModelInfo->GetRotate();

				fileTrans.write((const char*)&pos.x, sizeof(float));
				fileTrans.write((const char*)&pos.y, sizeof(float));
				fileTrans.write((const char*)&pos.z, sizeof(float));
				fileTrans.write((const char*)&scale.x, sizeof(float));
				fileTrans.write((const char*)&scale.y, sizeof(float));
				fileTrans.write((const char*)&scale.z, sizeof(float));

				fileTrans.write((const char*)&rot.x, sizeof(float));
				fileTrans.write((const char*)&rot.y, sizeof(float));
				fileTrans.write((const char*)&rot.z, sizeof(float));
			}

			fileTrans.close();
			ImGui::Begin("Save");
			ImGui::Text("Completed Save");
			ImGui::End();
		}
		if (GetKeyboardTrigger(DIK_UP))
		{
			if (animCheck_.AnimNumber > 0)
			{
				animCheck_.AnimNumber--;
				viewKey_ = 0;
				frame_ = 0;
				for (int partCount = 0; partCount < PART_MAX; partCount++)
				{
					parts[partCount].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[viewKey_].trans[partCount].rotate);
				}
			}
		}
		if (GetKeyboardTrigger(DIK_DOWN))
		{
			if (animCheck_.AnimNumber < ANIMTYPE_MAX - 1)
			{
				animCheck_.AnimNumber++;
				viewKey_ = 0;
				frame_ = 0;
				for (int partCount = 0; partCount < PART_MAX; partCount++)
				{
					parts[partCount].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[viewKey_].trans[partCount].rotate);
				}
			}
		}

		//登録したフレームを確認。
		if (GetKeyboardTrigger(DIK_LEFT))
		{
			if (motion_[animCheck_.AnimNumber].keynum > 0)
			{
				if (viewKey_ > 0)
				{
					viewKey_--;
					for (int partCount = 0; partCount < PART_MAX; partCount++)
					{
						parts[partCount].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[viewKey_].trans[partCount].rotate);
					}
				}
			}
		}
		//登録したフレームを確認
		if (GetKeyboardTrigger(DIK_RIGHT))
		{
			if (motion_[animCheck_.AnimNumber].keynum > 0)
			{
				if (viewKey_ < motion_[animCheck_.AnimNumber].keynum - 1)
				{
					viewKey_++;
					for (int partCount = 0; partCount < PART_MAX; partCount++)
					{
						parts[partCount].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[viewKey_].trans[partCount].rotate);
					}
				}

			}
		}
	}

	for (int i = 0; i < ANIMTYPE_MAX; i++)
	{
		for (UINT keyCount = 0; keyCount < motion_[i].keynum; keyCount++)
		{
			for (int partCount = 0; partCount < PART_MAX; partCount++)
			{
				ImGui::Begin("Anim Translation");
				ImGui::Text("%f %f %f", motion_[i].animdata[keyCount].trans[partCount].rotate.x, motion_[i].animdata[keyCount].trans[partCount].rotate.y, motion_[i].animdata[keyCount].trans[partCount].rotate.z);
				ImGui::End();
			}
		}
		
	
		
	}
	
	if (!animPlaying_)
	{
		if (ImGui::TreeNode("BODY"))
		{
			D3DXVECTOR3 rot = parts[BODY].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[BODY].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[BODY].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x,0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[BODY].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[BODY].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[BODY].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[BODY].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[BODY].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[BODY].min[2]);

			parts[BODY].ModelInfo->SetScale(scale);
			parts[BODY].ModelInfo->SetPosition(pos);
			parts[BODY].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("HEAD"))
		{
			D3DXVECTOR3 rot = parts[HEAD].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[HEAD].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[HEAD].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HEAD].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HEAD].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HEAD].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HEAD].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HEAD].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HEAD].min[2]);

			parts[HEAD].ModelInfo->SetScale(scale);
			parts[HEAD].ModelInfo->SetPosition(pos);
			parts[HEAD].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ARM_R"))
		{
			D3DXVECTOR3 rot = parts[ARM_R].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[ARM_R].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[ARM_R].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_R].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_R].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_R].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_R].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_R].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_R].min[2]);

			parts[ARM_R].ModelInfo->SetScale(scale);
			parts[ARM_R].ModelInfo->SetPosition(pos);
			parts[ARM_R].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ARM_L"))
		{
			D3DXVECTOR3 rot = parts[ARM_L].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[ARM_L].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[ARM_L].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_L].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_L].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_L].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_L].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_L].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ARM_L].min[2]);

			parts[ARM_L].ModelInfo->SetScale(scale);
			parts[ARM_L].ModelInfo->SetPosition(pos);
			parts[ARM_L].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("HAND_R"))
		{
			D3DXVECTOR3 rot = parts[HAND_R].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[HAND_R].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[HAND_R].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_R].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_R].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_R].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_R].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_R].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_R].min[2]);

			parts[HAND_R].ModelInfo->SetScale(scale);
			parts[HAND_R].ModelInfo->SetPosition(pos);
			parts[HAND_R].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("HAND_L"))
		{
			D3DXVECTOR3 rot = parts[HAND_L].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[HAND_L].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[HAND_L].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_L].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_L].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_L].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_L].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_L].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[HAND_L].min[2]);

			parts[HAND_L].ModelInfo->SetScale(scale);
			parts[HAND_L].ModelInfo->SetPosition(pos);
			parts[HAND_L].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ASI_R"))
		{
			D3DXVECTOR3 rot = parts[ASI_R].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[ASI_R].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[ASI_R].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &animData_[viewKey_].animRot[ASI_R].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &animData_[viewKey_].animRot[ASI_R].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &animData_[viewKey_].animRot[ASI_R].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_R].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_R].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_R].min[2]);

			parts[ASI_R].ModelInfo->SetScale(scale);
			parts[ASI_R].ModelInfo->SetPosition(pos);
			parts[ASI_R].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ASI_L"))
		{
			D3DXVECTOR3 rot = parts[ASI_L].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[ASI_L].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[ASI_L].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_L].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_L].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_L].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_L].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_L].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASI_L].min[2]);

			parts[ASI_L].ModelInfo->SetScale(scale);
			parts[ASI_L].ModelInfo->SetPosition(pos);
			parts[ASI_L].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ASIKUBI_R"))
		{
			D3DXVECTOR3 rot = parts[ASIKUBI_R].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[ASIKUBI_R].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[ASIKUBI_R].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_R].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_R].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_R].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_R].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_R].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_R].min[2]);

			parts[ASIKUBI_R].ModelInfo->SetScale(scale);
			parts[ASIKUBI_R].ModelInfo->SetPosition(pos);
			parts[ASIKUBI_R].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ASIKUBI_L"))
		{
			D3DXVECTOR3 rot = parts[ASIKUBI_L].ModelInfo->GetRotate();
			D3DXVECTOR3 pos = parts[ASIKUBI_L].ModelInfo->GetPosition();
			D3DXVECTOR3 scale = parts[ASIKUBI_L].ModelInfo->GetScale();
			ImGui::DragFloat("POS X", &pos.x, 0.01f);
			ImGui::DragFloat("POS Y", &pos.y, 0.01f);
			ImGui::DragFloat("POS Z", &pos.z, 0.01f);
			ImGui::DragFloat("SCALE X", &scale.x, 0.01f);
			ImGui::DragFloat("SCALE Y", &scale.y, 0.01f);
			ImGui::DragFloat("SCALE Z", &scale.z, 0.01f);
			ImGui::DragFloat("ROT X", &rot.x, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Y", &rot.y, 0, 360.0f, 0.01f);
			ImGui::DragFloat("ROT Z", &rot.z, 0, 360.0f, 0.01f);
			ImGui::Checkbox("RotateRightX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_L].rightRot[0]);
			ImGui::Checkbox("RotateRightY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_L].rightRot[1]);
			ImGui::Checkbox("RotateRightZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_L].rightRot[2]);
			ImGui::Checkbox("MinX", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_L].min[0]);
			ImGui::Checkbox("MinY", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_L].min[1]);
			ImGui::Checkbox("MinZ", &motion_[animCheck_.AnimNumber].animdata[viewKey_].animRot[ASIKUBI_L].min[2]);

			parts[ASIKUBI_L].ModelInfo->SetScale(scale);
			parts[ASIKUBI_L].ModelInfo->SetPosition(pos);
			parts[ASIKUBI_L].ModelInfo->SetRotation(rot);
			ImGui::TreePop();
		}
	}
	
	
	ImGui::End();
}

void AnimPlayer::AnimationPlay()
{
	if (animPlaying_)
	{
		if (motion_[animCheck_.AnimNumber].keynum > 1)
		{
			nowFrame++;
			
			//アニメーション繰り返し処理
			if (nowAnimKey_ == (motion_[animCheck_.AnimNumber].keynum - 2) && nowFrame > motion_[animCheck_.AnimNumber].animdata[motion_[animCheck_.AnimNumber].keynum - 1].frame)
			{
				nowFrame = 0;
				nowAnimKey_ = 0;
				for (int i = 0; i < PART_MAX; i++)
				{
					parts[i].ModelInfo->SetRotation(motion_[animCheck_.AnimNumber].animdata[0].trans[i].rotate);
				}
			}
			if (nowFrame > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].frame)
			{
				nowAnimKey_++;
			}

			for (int i = 0; i < PART_MAX; i++)
			{
				if (nowAnimKey_ < motion_[animCheck_.AnimNumber].keynum - 1)
				{
					if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].rightRot[0])			//X軸
					{
						float angle;
						if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].min[0])
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x;
								}*/
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x;
								}*/
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
							if (angle > 180.0f)
							{
								angle = 360.0f - angle;
							}
						}
						else
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x;
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x;
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
						}
						
						

						//キー間のフレーム数を求める。
						int frame = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].frame - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].frame;

						//1フレームあたりの回転角を求める。
						animRotate_[i].x = ((float)angle / frame);
					}
					else
					{
						float angle;
						if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].min[0])
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x;
								}*/
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x;
								}*/
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
							if (angle > 180.0f)
							{
								angle = 360.0f - angle;
							}
						}
						else
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x;
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.x - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.x;
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
						}

						//キー間のフレーム数を求める。
						int frame = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].frame - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].frame;

						//1フレームあたりの回転角を求める。
						animRotate_[i].x = ((float)angle / frame) * -1.0f;
					}
					if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].rightRot[1])			//Y軸
					{
						float angle;
						if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].min[1])
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y;
								}*/
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y;
								}*/
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
							if (angle > 180.0f)
							{
								angle = 360.0f - angle;
							}
						}
						else
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y;
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y;
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
						}

						//キー間のフレーム数を求める。
						int frame = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].frame - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].frame;

						//1フレームあたりの回転角を求める。
						animRotate_[i].y = ((float)angle / frame);
					}
					else
					{
						float angle;
						if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].min[1])
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y;
								}*/
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y;
								}*/
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
							if (angle > 180.0f)
							{
								angle = 360.0f - angle;
							}
						}
						else
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y;
								
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.y - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.y;
								
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
						}

						//キー間のフレーム数を求める。
						int frame = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].frame - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].frame;

						//1フレームあたりの回転角を求める。
						animRotate_[i].y = ((float)angle / frame) * -1.0f;
					}
					if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].rightRot[2])			//Z軸
					{
						float angle;
						if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].min[2])
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z;
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z;
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
							if (angle > 180.0f)
							{
								angle = 360.0f - angle;
							}
						}
						else
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z;
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z;
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
						}

						//キー間のフレーム数を求める。
						int frame = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].frame - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].frame;

						//1フレームあたりの回転角を求める。
						animRotate_[i].z = ((float)angle / frame);
					}
					else
					{
						float angle;
						if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].animRot[i].min[2])
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z;
								}*/
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z;
								/*if (angle > 180.0f)
								{
									angle = (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z + 360.0f) - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z;
								}*/
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
							if (angle > 180.0f)
							{
								angle = 360.0f - angle;
							}
						}
						else
						{
							if (motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z > motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z)
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z;
							}
							else
							{
								//合計の回転角度を求める。
								angle = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].trans[i].rotate.z - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].trans[i].rotate.z;	
							}
							if (angle < 0)
							{
								angle *= -1.0f;
							}
						}

						//キー間のフレーム数を求める。
						int frame = motion_[animCheck_.AnimNumber].animdata[nowAnimKey_ + 1].frame - motion_[animCheck_.AnimNumber].animdata[nowAnimKey_].frame;

						//1フレームあたりの回転角を求める。
						animRotate_[i].z = ((float)angle / frame) * -1.0f;
					}
				}

			}

			//パーツの更新
			for (int i = 0; i < PART_MAX; i++)
			{
				auto rot = parts[i].ModelInfo->GetRotate();
				rot.x += animRotate_[i].x;
				rot.y += animRotate_[i].y;
				rot.z += animRotate_[i].z;
				parts[i].ModelInfo->SetRotation(rot);
			}

		}


	}
}



