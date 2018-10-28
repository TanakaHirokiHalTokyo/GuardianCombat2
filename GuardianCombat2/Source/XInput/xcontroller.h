//**-------------------------------------------------------**
//**
//**                  xcontroller.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _XCONTROLLER_H_
#define _XCONTROLLER_H_

#include <Xinput.h>

constexpr UINT MAX_CONTROLLERS = 1;

class X_CONTROLLER
{
private:
	struct XCONT_BUTTON_STATUS
	{
		bool DPAD_UP;
		bool DPAD_DOWN;
		bool DPAD_LEFT;
		bool DPAD_RIGHT;
		bool START;
		bool BACK;
		bool LEFT_THUMB;
		bool RIGHT_THUMB;
		bool LEFT_SHOULDER;
		bool RIGHT_SHOULDER;
		bool A;
		bool B;
		bool X;
		bool Y;
	};

	static XCONT_BUTTON_STATUS buttonStatus_[MAX_CONTROLLERS];
	static XCONT_BUTTON_STATUS buttonStatusOld_[MAX_CONTROLLERS];

	static XINPUT_STATE controllerState_[MAX_CONTROLLERS];
	static bool controller_;

public:
	static void	InitXcontroller();
	static void	UninitXcontroller();
	static void	UpdateXcontroller();
	static void	UpdateXcontrollerCreateOld();
	static void	XcontrollerValue(int nNumController);
	
	static double	GetXcontrollerLStickX(int nNumController);
	static double	GetXcontrollerLStickY(int nNumController);
	static double	GetXcontrollerRStickX(int nNumController);
	static double	GetXcontrollerRStickY(int nNumController);
	static double	GetXcontrollerLTrigger(int nNumController);
	static double	GetXcontrollerRTrigger(int nNumController);
	static bool	GetXcontrollerButtonPress(int nNumController, WORD button);
	static bool	GetXcontrollerButtonTrigger(int nNumController, WORD button);
	static bool	GetXcontrollerButtonRelease(int nNumController, WORD button);
	static bool	GetConnectController();
};

#endif// !_XCONTROLLER_H_