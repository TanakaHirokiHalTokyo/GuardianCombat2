#pragma once

class DeviceConfig
{
public:
	const float MouseMaxSensitivity = 0.2f;
	const float ControllerMaxSensitivity = 1.0f;

	const float MouseMinSensitivity = 0.01f;
	const float ControllerMinSensitivity = 0.1f;

	DeviceConfig();

	void ChangeMouseSensitivity(float Sensi);
	void ChangeControllerSensitivity(float Sensi);

	float GetMouseSensitivity();
	float GetControllerSensitivity();
private:
	float mouseSensitivity_ = MouseMinSensitivity;
	float controllerSensitivity_ = ControllerMinSensitivity;


};