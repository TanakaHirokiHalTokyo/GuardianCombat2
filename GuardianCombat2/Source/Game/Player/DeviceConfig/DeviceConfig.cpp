#include "DeviceConfig.h"

void DeviceConfig::ChangeMouseSensitivity(float Sensi)
{
	mouseSensitivity_ = Sensi;
}

void DeviceConfig::ChangeControllerSensitivity(float Sensi)
{
	controllerSensitivity_ = Sensi;
}

float DeviceConfig::GetMouseSensitivity()
{
	return mouseSensitivity_;
}

float DeviceConfig::GetControllerSensitivity()
{
	return controllerSensitivity_;
}
