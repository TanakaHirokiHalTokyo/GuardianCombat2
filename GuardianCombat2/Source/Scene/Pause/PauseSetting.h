//===================================================================
//
//		PauseSetting マウス・コントローラ感度変更など
//
//===================================================================
#pragma once

class Texture;
class DeviceConfig;
class PauseSelect;

#include <string>

class PauseSetting
{
public:
	static const std::string SENCITIVITY_FILENAME;

	enum DEVICE
	{
		MOUSE = 1,
		CONTROLLER
	};
	struct Size
	{
		float x;
		float y;
	};
	struct Position
	{
		float x;
		float y;
	};

	PauseSetting(PauseSelect* select);
	~PauseSetting();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	inline void ChangeDevice(DEVICE dev) { device_ = dev; }			//選択デバイス変更
	void SetCirclePosition();			//選択したデバイスの感度割合をもとに丸の座標設定

	void SaveSensitivity(std::string filename);		//感度情報保存
private:
	void SaveSettings(std::string filename);			//設定した値保存
	void LoadSettings(std::string filename);			//設定した値読込
	void LoadSensitivity(std::string filename);		//感度情報読込

	float mouse_rate_ = 1.0f;						//マウス感度割合
	float controller_rate_ = 1.0f;						//コントローラ感度割合
	PauseSelect* select_ = nullptr;				//Select情報
	DeviceConfig* config_ = nullptr;				//デバイス感度情報
	DEVICE device_ = MOUSE;					//選択中のデバイス
	Texture* bar_ = nullptr;							//スライダーバー
	Texture* circle_ = nullptr;						//丸テクスチャ
	Texture* b_ = nullptr;								//Bテクスチャ
	Texture* back_ = nullptr;						//Backテクスチャ
	Position bPosition_ = { 0.0f };					//Bの座標
	Size bSize_ = { 0.0f };								//Bのサイズ
	float bColor_[3] = { 0 };							//Bの色
	Position backPosition_ = { 0.0f };			//Backの座標
	Size backSize_ = { 0.0f };					//Backのサイズ
	float backColor_[3] = { 0 };						//Backの色
	Position barPosition_ = { 0.0f };				//バーの座標
	Size barSize_ = { 0.0f };							//バーの大きさ
	float barColor_[3] = { 0 };						//バーの色
	Position circlePosition_ = { 0.0f };			//丸の座標
	Size circleSize_ = { 0.0f };						//丸の大きさ
	float circleColor_[3] = { 0 };					//丸の色
	bool isClicked_ = false;							//クリックしてボタンを動かしているのか
	float mouseStartX_ = 0.0f;						//クリックした時点でのマウスX座標
};
