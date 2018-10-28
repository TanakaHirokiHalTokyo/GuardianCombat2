#include "UIState_Scale.h"
#include "../../Texture/Texture.h"
#include "../UI.h"
#include "../../main.h"

void UIState_Scale::Update(UI * ui)
{
	if (updateScale_)
	{
		//Get Texture
		auto texture = ui->GetTexture();
		//Get UIDrawSize
		const auto& ui_position = ui->GetScale();
		//GetCountMove
		const auto& count_scale = ui->GetCountScale();
		//GetMoveTime
		const auto& scale_time = ui->GetScaleTime();
		//GetUpdatePos
		const auto& update_size = ui->GetAfterScale();

		//移動開始させるか
		if (count_ >= count_scale * GameFPS)
		{
			if (!initScaleValue_)
			{
				//現在の差分を計算
				moveValue_.x = update_size.x - ui_position.x;
				moveValue_.y = update_size.y - ui_position.y;

				float time = (float)GameFPS * (scale_time + count_scale);
				time = time - count_;

				moveValue_.x = moveValue_.x / time;
				moveValue_.y = moveValue_.y / time;

				initScaleValue_ = true;
			}

			ui->SetScale(ui_position.x + moveValue_.x, ui_position.y + moveValue_.y, 0.0f);
		}

		//移動更新を終えるか
		if (count_ >= GameFPS * (scale_time + count_scale))
		{
			updateScale_ = false;
		}

		//Update Count
		count_++;
	}
	
}
