#include "UIState_Move.h"
#include "../../Texture/Texture.h"
#include "../UI.h"
#include "../../main.h"

void UIState_Move::Update(UI* ui)
{
	if (updateMove_)
	{
		//Get Texture
		auto texture = ui->GetTexture();
		//Get UIPosition
		const auto& ui_position = ui->GetPosition();
		//GetCountMove
		const auto& count_move = ui->GetCountMove();
		//GetMoveTime
		const auto& move_time = ui->GetMoveTime();
		//GetUpdatePos
		const auto& update_pos = ui->GetAfterPos();

		//移動開始させるか
		if (count_ >= count_move * GameFPS)
		{
			if (!initMoveValue_)
			{
				//現在の差分を計算
				moveValue_.x = update_pos.x - ui_position.x;
				moveValue_.y = update_pos.y - ui_position.y;

				float time = (float)GameFPS * (move_time + count_move);
				time = time - count_;

				moveValue_.x = moveValue_.x / time;
				moveValue_.y = moveValue_.y / time;

				initMoveValue_ = true;
			}

			ui->SetPosition(ui_position.x + moveValue_.x, ui_position.y + moveValue_.y, 0.0f);
		}

		//移動更新を終えるか
		if (count_ >= GameFPS * (move_time + count_move))
		{
			updateMove_ = false;
		}

		//Update Count
		count_++;
	}
}
