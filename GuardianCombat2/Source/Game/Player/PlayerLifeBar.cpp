#include "PlayerLifeBar.h"
#include "../../Texture/Texture.h"
#include "../../main.h"
#include "Player.h"

//èâä˙ï`âÊÉTÉCÉY
const D3DXVECTOR2 InitDrawSize = D3DXVECTOR2((float)ScreenWidth / 2.0f, (float)ScreenHeight / 5.0f);

PlayerLifeBar::PlayerLifeBar(Player* player)
{
	texture_ = Object::Create<Texture>(TextureManager::Tex_SliderBar);
	player_ = player;
}

PlayerLifeBar::~PlayerLifeBar()
{
}

void PlayerLifeBar::Init()
{
	texture_->SetDrawSize(InitDrawSize.x, InitDrawSize.y);
	texture_->SetPosition(0,0,0);
	texture_->SetColor(D3DCOLOR_RGBA(100,255,100,200));
	texture_->SetVisible(true);
}

void PlayerLifeBar::Uninit()
{
}

void PlayerLifeBar::Update()
{
	float rate = player_->GetLife() / PLAYER_DEFAULT_LIFE;
	texture_->SetDrawSize(InitDrawSize.x * rate,InitDrawSize.y);
}

void PlayerLifeBar::BeginDraw()
{
}

void PlayerLifeBar::Draw()
{
}

void PlayerLifeBar::EndDraw()
{
}
