//**-------------------------------------------------------**
//**
//**                  xcontroller.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <Windows.h>

#include "../Imgui/ImguiManager.h"

#include "xcontroller.h"

constexpr double LEFT_STICK_DEADZONE = 3000.0;
constexpr double RIGHT_STICK_DEADZONE = 3000.0;
constexpr double TRIGGER_DEADZONE = 20.0;

X_CONTROLLER::XCONT_BUTTON_STATUS X_CONTROLLER::buttonStatus_[MAX_CONTROLLERS];
X_CONTROLLER::XCONT_BUTTON_STATUS X_CONTROLLER::buttonStatusOld_[MAX_CONTROLLERS];

XINPUT_STATE X_CONTROLLER::controllerState_[MAX_CONTROLLERS];
bool X_CONTROLLER::controller_ = false;

void X_CONTROLLER::InitXcontroller()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		buttonStatus_[i].DPAD_UP = false;
		buttonStatus_[i].DPAD_DOWN = false;
		buttonStatus_[i].DPAD_LEFT = false;
		buttonStatus_[i].DPAD_RIGHT = false;
		buttonStatus_[i].START = false;
		buttonStatus_[i].BACK = false;
		buttonStatus_[i].LEFT_THUMB = false;
		buttonStatus_[i].RIGHT_THUMB = false;
		buttonStatus_[i].LEFT_SHOULDER = false;
		buttonStatus_[i].RIGHT_SHOULDER = false;
		buttonStatus_[i].A = false;
		buttonStatus_[i].B = false;
		buttonStatus_[i].X = false;
		buttonStatus_[i].Y = false;

		buttonStatusOld_[i].DPAD_UP = false;
		buttonStatusOld_[i].DPAD_DOWN = false;
		buttonStatusOld_[i].DPAD_LEFT = false;
		buttonStatusOld_[i].DPAD_RIGHT = false;
		buttonStatusOld_[i].START = false;
		buttonStatusOld_[i].BACK = false;
		buttonStatusOld_[i].LEFT_THUMB = false;
		buttonStatusOld_[i].RIGHT_THUMB = false;
		buttonStatusOld_[i].LEFT_SHOULDER = false;
		buttonStatusOld_[i].RIGHT_SHOULDER = false;
		buttonStatusOld_[i].A = false;
		buttonStatusOld_[i].B = false;
		buttonStatusOld_[i].X = false;
		buttonStatusOld_[i].Y = false;
	}
}

void X_CONTROLLER::UninitXcontroller()
{
	XInputEnable(false);
}

void X_CONTROLLER::UpdateXcontroller()
{
	DWORD dwResult;

	controller_ = false;

	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		ZeroMemory(&controllerState_[i], sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &controllerState_[i]);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
			controller_ = true;
		}

		//コントローラー状態取得
		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			buttonStatus_[i].DPAD_UP = true;
		}
		else
		{
			buttonStatus_[i].DPAD_UP = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			buttonStatus_[i].DPAD_DOWN = true;
		}
		else
		{
			buttonStatus_[i].DPAD_DOWN = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			buttonStatus_[i].DPAD_LEFT = true;
		}
		else
		{
			buttonStatus_[i].DPAD_LEFT = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			buttonStatus_[i].DPAD_RIGHT = true;
		}
		else
		{
			buttonStatus_[i].DPAD_RIGHT = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			buttonStatus_[i].START = true;
		}
		else
		{
			buttonStatus_[i].START = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			buttonStatus_[i].BACK = true;
		}
		else
		{
			buttonStatus_[i].BACK = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			buttonStatus_[i].LEFT_THUMB = true;
		}
		else
		{
			buttonStatus_[i].LEFT_THUMB = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			buttonStatus_[i].RIGHT_THUMB = true;
		}
		else
		{
			buttonStatus_[i].RIGHT_THUMB = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			buttonStatus_[i].LEFT_SHOULDER = true;
		}
		else
		{
			buttonStatus_[i].LEFT_SHOULDER = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			buttonStatus_[i].RIGHT_SHOULDER = true;
		}
		else
		{
			buttonStatus_[i].RIGHT_SHOULDER = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			buttonStatus_[i].A = true;
		}
		else
		{
			buttonStatus_[i].A = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			buttonStatus_[i].B = true;
		}
		else
		{
			buttonStatus_[i].B = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			buttonStatus_[i].X = true;
		}
		else
		{
			buttonStatus_[i].X = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			buttonStatus_[i].Y = true;
		}
		else
		{
			buttonStatus_[i].Y = false;
		}

		//LStickX
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbLX < LEFT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbLX > -LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLX = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbLX >= LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLX -= (SHORT)LEFT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbLX <= (SHORT)-LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLX += (SHORT)LEFT_STICK_DEADZONE;
		}

		//LStickY
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbLY < LEFT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbLY > -LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLY = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbLY >= (SHORT)LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLY -= (SHORT)LEFT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbLY <= (SHORT)-LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLY += (SHORT)LEFT_STICK_DEADZONE;
		}

		//RStickX
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbRX < RIGHT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbRX > -RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRX = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbRX >= RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRX -= (SHORT)RIGHT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbRX <= (SHORT)-RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRX += (SHORT)RIGHT_STICK_DEADZONE;
		}

		//RStickY
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbRY < RIGHT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbRY > -RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRY = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbRY >= (SHORT)RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRY -= (SHORT)RIGHT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbRY <= (SHORT)-RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRY += (SHORT)RIGHT_STICK_DEADZONE;
		}
	}
}

void X_CONTROLLER::UpdateXcontrollerCreateOld()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		buttonStatusOld_[i] = buttonStatus_[i];
	}
}

void X_CONTROLLER::XcontrollerValue(int nNumController)
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.4f, 0.6f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.4f, 0.6f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(340, 420), ImGuiSetCond_Once);

	ImGui::Begin("XController");
	if (GetConnectController())
	{
		ImGui::Text("Connected");
	}
	else
	{
		ImGui::Text("No Connection");
	}

	ImGui::Text("Controller %d", nNumController);
	ImGui::Text("LeftStick X : %6d (%.6f)", controllerState_[nNumController - 1].Gamepad.sThumbLX, (controllerState_[nNumController - 1].Gamepad.sThumbLX - (-32768)) / 32768.0f - 1.0f);
	ImGui::Text("LeftStick Y : %6d (%.6f)", controllerState_[nNumController - 1].Gamepad.sThumbLY, (controllerState_[nNumController - 1].Gamepad.sThumbLY - (-32768)) / 32768.0f - 1.0f);
	ImGui::Text("RightStick X : %6d (%.6f)", controllerState_[nNumController - 1].Gamepad.sThumbRX, (controllerState_[nNumController - 1].Gamepad.sThumbRX - (-32768)) / 32768.0f - 1.0f);
	ImGui::Text("RightStick Y : %6d (%.6f)", controllerState_[nNumController - 1].Gamepad.sThumbRY, (controllerState_[nNumController - 1].Gamepad.sThumbRY - (-32768)) / 32768.0f - 1.0f);
	ImGui::Text("LeftTrigger : %6d (%.6f)", controllerState_[nNumController - 1].Gamepad.bLeftTrigger, (controllerState_[nNumController - 1].Gamepad.bLeftTrigger - 0) / 255.0f);
	ImGui::Text("RightTrigger : %6d (%.6f)", controllerState_[nNumController - 1].Gamepad.bRightTrigger, (controllerState_[nNumController - 1].Gamepad.bRightTrigger - 0) / 255.0f);

	if (buttonStatus_[nNumController - 1].DPAD_UP)
	{
		ImGui::Text("DPAD_UP : TRUE");
	}
	else
	{
		ImGui::Text("DPAD_UP : FALSE");
	}
	if (buttonStatus_[nNumController - 1].DPAD_DOWN)
	{
		ImGui::Text("DPAD_DOWN : TRUE");
	}
	else
	{
		ImGui::Text("DPAD_DOWN : FALSE");
	}
	if (buttonStatus_[nNumController - 1].DPAD_LEFT)
	{
		ImGui::Text("DPAD_LEFT : TRUE");
	}
	else
	{
		ImGui::Text("DPAD_LEFT : FALSE");
	}
	if (buttonStatus_[nNumController - 1].DPAD_RIGHT)
	{
		ImGui::Text("DPAD_RIGHT : TRUE");
	}
	else
	{
		ImGui::Text("DPAD_RIGHT : FALSE");
	}
	if (buttonStatus_[nNumController - 1].START)
	{
		ImGui::Text("START : TRUE");
	}
	else
	{
		ImGui::Text("START : FALSE");
	}
	if (buttonStatus_[nNumController - 1].BACK)
	{
		ImGui::Text("BACK : TRUE");
	}
	else
	{
		ImGui::Text("BACK : FALSE");
	}
	if (buttonStatus_[nNumController - 1].LEFT_THUMB)
	{
		ImGui::Text("LEFT_THUMB : TRUE");
	}
	else
	{
		ImGui::Text("LEFT_THUMB : FALSE");
	}
	if (buttonStatus_[nNumController - 1].RIGHT_THUMB)
	{
		ImGui::Text("RIGHT_THUMB : TRUE");
	}
	else
	{
		ImGui::Text("RIGHT_THUMB : FALSE");
	}
	if (buttonStatus_[nNumController - 1].LEFT_SHOULDER)
	{
		ImGui::Text("LEFT_SHOULDER : TRUE");
	}
	else
	{
		ImGui::Text("LEFT_SHOULDER : FALSE");
	}
	if (buttonStatus_[nNumController - 1].RIGHT_SHOULDER)
	{
		ImGui::Text("RIGHT_SHOULDER : TRUE");
	}
	else
	{
		ImGui::Text("RIGHT_SHOULDER : FALSE");
	}
	if (buttonStatus_[nNumController - 1].A)
	{
		ImGui::Text("A : TRUE");
	}
	else
	{
		ImGui::Text("A : FALSE");
	}
	if (buttonStatus_[nNumController - 1].B)
	{
		ImGui::Text("B : TRUE");
	}
	else
	{
		ImGui::Text("B : FALSE");
	}
	if (buttonStatus_[nNumController - 1].X)
	{
		ImGui::Text("X : TRUE");
	}
	else
	{
		ImGui::Text("X : FALSE");
	}
	if (buttonStatus_[nNumController - 1].Y)
	{
		ImGui::Text("Y : TRUE");
	}
	else
	{
		ImGui::Text("Y : FALSE");
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

double X_CONTROLLER::GetXcontrollerLStickX(int nNumController)
{
	//NormalizeReturn
	return (controllerState_[nNumController - 1].Gamepad.sThumbLX - (-(32768 - LEFT_STICK_DEADZONE))) / (32768.0f - LEFT_STICK_DEADZONE) - 1.0f;
}

double X_CONTROLLER::GetXcontrollerLStickY(int nNumController)
{
	//NormalizeReturn
	return (controllerState_[nNumController - 1].Gamepad.sThumbLY - (-(32768 - LEFT_STICK_DEADZONE))) / (32768.0f - LEFT_STICK_DEADZONE) - 1.0f;
}

double X_CONTROLLER::GetXcontrollerRStickX(int nNumController)
{
	//NormalizeReturn
	return (controllerState_[nNumController - 1].Gamepad.sThumbRX - (-(32768 - RIGHT_STICK_DEADZONE))) / (32768.0f - RIGHT_STICK_DEADZONE) - 1.0f;
}

double X_CONTROLLER::GetXcontrollerRStickY(int nNumController)
{
	//NormalizeReturn
	return (controllerState_[nNumController - 1].Gamepad.sThumbRY - (-(32768 - RIGHT_STICK_DEADZONE))) / (32768.0f - RIGHT_STICK_DEADZONE) - 1.0f;
}

double X_CONTROLLER::GetXcontrollerLTrigger(int nNumController)
{
	//DeadZone
	if (controllerState_[nNumController - 1].Gamepad.bLeftTrigger < TRIGGER_DEADZONE)
	{
		controllerState_[nNumController - 1].Gamepad.bLeftTrigger = (SHORT)0.0f;
	}
	
	//NormalizeReturn
	return (controllerState_[nNumController - 1].Gamepad.bLeftTrigger - 0) / 255.0f;
}

double X_CONTROLLER::GetXcontrollerRTrigger(int nNumController)
{
	//DeadZone
	if (controllerState_[nNumController - 1].Gamepad.bRightTrigger < TRIGGER_DEADZONE)
	{
		controllerState_[nNumController - 1].Gamepad.bRightTrigger = (SHORT)0.0f;
	}

	//NormalizeReturn
	return (controllerState_[nNumController - 1].Gamepad.bRightTrigger - 0) / 255.0f;
}

bool X_CONTROLLER::GetXcontrollerButtonPress(int nNumController, WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		return buttonStatus_[nNumController - 1].DPAD_UP;
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		return buttonStatus_[nNumController - 1].DPAD_DOWN;
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		return buttonStatus_[nNumController - 1].DPAD_LEFT;
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		return buttonStatus_[nNumController - 1].DPAD_RIGHT;
		break;
	case XINPUT_GAMEPAD_START:
		return buttonStatus_[nNumController - 1].START;
		break;
	case XINPUT_GAMEPAD_BACK:
		return buttonStatus_[nNumController - 1].BACK;
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		return buttonStatus_[nNumController - 1].LEFT_THUMB;
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		return buttonStatus_[nNumController - 1].RIGHT_THUMB;
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		return buttonStatus_[nNumController - 1].LEFT_SHOULDER;
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		return buttonStatus_[nNumController - 1].RIGHT_SHOULDER;
		break;
	case XINPUT_GAMEPAD_A:
		return buttonStatus_[nNumController - 1].A;
		break;
	case XINPUT_GAMEPAD_B:
		return buttonStatus_[nNumController - 1].B;
		break;
	case XINPUT_GAMEPAD_X:
		return buttonStatus_[nNumController - 1].X;
		break;
	case XINPUT_GAMEPAD_Y:
		return buttonStatus_[nNumController - 1].Y;
		break;
	default:
		return false;
		break;
	}
}
bool X_CONTROLLER::GetXcontrollerButtonTrigger(int nNumController, WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		if (buttonStatusOld_[nNumController - 1].DPAD_UP == false)
		{
			return buttonStatus_[nNumController - 1].DPAD_UP;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		if (buttonStatusOld_[nNumController - 1].DPAD_DOWN == false)
		{
			return buttonStatus_[nNumController - 1].DPAD_DOWN;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		if (buttonStatusOld_[nNumController - 1].DPAD_LEFT == false)
		{
			return buttonStatus_[nNumController - 1].DPAD_LEFT;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		if (buttonStatusOld_[nNumController - 1].DPAD_RIGHT == false)
		{
			return buttonStatus_[nNumController - 1].DPAD_RIGHT;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_START:
		if (buttonStatusOld_[nNumController - 1].START == false)
		{
			return buttonStatus_[nNumController - 1].START;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_BACK:
		if (buttonStatusOld_[nNumController - 1].BACK == false)
		{
			return buttonStatus_[nNumController - 1].BACK;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		if (buttonStatusOld_[nNumController - 1].LEFT_THUMB == false)
		{
			return buttonStatus_[nNumController - 1].LEFT_THUMB;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		if (buttonStatusOld_[nNumController - 1].RIGHT_THUMB == false)
		{
			return buttonStatus_[nNumController - 1].RIGHT_THUMB;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		if (buttonStatusOld_[nNumController - 1].LEFT_SHOULDER == false)
		{
			return buttonStatus_[nNumController - 1].LEFT_SHOULDER;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		if (buttonStatusOld_[nNumController - 1].RIGHT_SHOULDER == false)
		{
			return buttonStatus_[nNumController - 1].RIGHT_SHOULDER;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_A:
		if (buttonStatusOld_[nNumController - 1].A == false)
		{
			return buttonStatus_[nNumController - 1].A;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_B:
		if (buttonStatusOld_[nNumController - 1].B == false)
		{
			return buttonStatus_[nNumController - 1].B;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_X:
		if (buttonStatusOld_[nNumController - 1].X == false)
		{
			return buttonStatus_[nNumController - 1].X;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_Y:
		if (buttonStatusOld_[nNumController - 1].Y == false)
		{
			return buttonStatus_[nNumController - 1].Y;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
		break;
	}
}
bool X_CONTROLLER::GetXcontrollerButtonRelease(int nNumController, WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		if (buttonStatusOld_[nNumController - 1].DPAD_UP == true && buttonStatus_[nNumController - 1].DPAD_UP == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		if (buttonStatusOld_[nNumController - 1].DPAD_DOWN == true && buttonStatus_[nNumController - 1].DPAD_DOWN == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		if (buttonStatusOld_[nNumController - 1].DPAD_LEFT == true && buttonStatus_[nNumController - 1].DPAD_LEFT == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		if (buttonStatusOld_[nNumController - 1].DPAD_RIGHT == true && buttonStatus_[nNumController - 1].DPAD_RIGHT == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_START:
		if (buttonStatusOld_[nNumController - 1].START == true && buttonStatus_[nNumController - 1].START == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_BACK:
		if (buttonStatusOld_[nNumController - 1].BACK == true && buttonStatus_[nNumController - 1].BACK == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		if (buttonStatusOld_[nNumController - 1].LEFT_THUMB == true && buttonStatus_[nNumController - 1].LEFT_THUMB == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		if (buttonStatusOld_[nNumController - 1].RIGHT_THUMB == true && buttonStatus_[nNumController - 1].RIGHT_THUMB == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		if (buttonStatusOld_[nNumController - 1].LEFT_SHOULDER == true && buttonStatus_[nNumController - 1].LEFT_SHOULDER == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		if (buttonStatusOld_[nNumController - 1].RIGHT_SHOULDER == true && buttonStatus_[nNumController - 1].RIGHT_SHOULDER == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_A:
		if (buttonStatusOld_[nNumController - 1].A == true && buttonStatus_[nNumController - 1].A == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_B:
		if (buttonStatusOld_[nNumController - 1].B == true && buttonStatus_[nNumController - 1].B == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_X:
		if (buttonStatusOld_[nNumController - 1].X == true && buttonStatus_[nNumController - 1].X == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_Y:
		if (buttonStatusOld_[nNumController - 1].Y == true && buttonStatus_[nNumController - 1].Y == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
		break;
	}
}

bool	X_CONTROLLER::GetConnectController()
{
	return controller_;
}