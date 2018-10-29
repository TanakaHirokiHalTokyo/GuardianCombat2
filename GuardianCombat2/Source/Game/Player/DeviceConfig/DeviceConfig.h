#pragma once

class DeviceConfig
{
public:
	const float MouseMaxSensitivity = 2.0f;
	const float ControllerMaxSensitivity = 5.0f;

	const float MouseMinSensitivity = 0.01f;
	const float ControllerMinSensitivity = 0.1f;

	void ChangeMouseSensitivity(float Sensi);
	void ChangeControllerSensitivity(float Sensi);

	float GetMouseSensitivity();
	float GetControllerSensitivity();
private:
	float mouseSensitivity_ = 0.1f;
	float controllerSensitivity_ = 2.0f;


};