#pragma once

#define DIRECTINPUT_VERSION 0x0800 
#include <dinput.h>
#include <XInput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xinput.lib")
#include <wrl.h>

#include <vector>
#include "Utils/Math/Vector2.h"
#include "WinApp/WinApp.h"

class Input
{
public:

	enum class GamePadButton {
		A,
		B,
		X,
		Y,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		START,
		BACK,
		LEFT_THUMB,
		RIGHT_THUMB,
		LEFT_SHOULDER,
		RIGHT_SHOULDER,
	};

	enum class MouseButton {
		LEFT,
		RIGHT,
		CENTER,
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Input* GetInstance();

	void Initialize();

	void Update();

	/// <summary>
	/// キーボードが押された瞬間
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PressedKey(BYTE keyNumber) const;

	/// <summary>
	/// キーボードが押されている間
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押され続けているか</returns>
	bool PressingKey(BYTE keyNumber) const;

	/// <summary>
	/// キーボードを離した瞬間
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>離したか</returns>
	bool ReleasedKey(BYTE keyNumber) const;


	/// <summary>
	/// ゲームパッドのLスティックの入力
	/// </summary>
	/// <returns>Vector2</returns>
	Vector2 GetGamePadLStick();

	/// <summary>
	/// ゲームパッドのRスティックの入力
	/// </summary>
	/// <returns>Vector2</returns>
	Vector2 GetGamePadRStick();

	/// <summary>
	/// ゲームパッドが押された瞬間
	/// </summary>
	/// <param name="button">ボタン( Input::GamePadButton::A 等)</param>
	/// <returns>押されているか</returns>
	bool PressedGamePadButton(GamePadButton button);

	/// <summary>
	/// ゲームパッドが押され続けているか
	/// </summary>
	/// <param name="button">ボタン( Input::GamePadButton::A 等)</param>
	/// <returns>押され続けているか</returns>
	bool PressingGamePadButton(GamePadButton button);

	/// <summary>
	/// ゲームパッドが離れてか
	/// </summary>
	/// <param name="button">ボタン( Input::GamePadButton::A 等)</param>
	/// <returns>離れたか</returns>
	bool ReleasedGamePadButton(GamePadButton button);

	/// <summary>
	/// マウスが押された瞬間
	/// </summary>
	/// <param name="button">ボタン( Input::MouseButton::LEFT 等)</param>
	/// <returns>押されているか</returns>
	bool PressedMouse(MouseButton button);

	/// <summary>
	/// マウスが押され続けているか
	/// </summary>
	/// <param name="button">ボタン( Input::MouseButton::LEFT 等)</param>
	/// <returns>押され続けているか</returns>
	bool PressingMouse(MouseButton button);

	/// <summary>
	/// マウスが離れたか
	/// </summary>
	/// <param name="button">ボタン( Input::MouseButton::LEFT 等)</param>
	/// <returns>離れたか</returns>
	bool ReleasedMouse(MouseButton button);

	/// <summary>
	/// マウスホイールのスクロール量を取得する
	/// </summary>
	/// <returns>ホイールのスクロール量</returns>
	int32_t GetWheel();

	/// <summary>
	/// マウスの動量を取得する
	/// </summary>
	/// <returns>マウスの移動量</returns>
	Vector2 GetMouseMove();

	/// <summary>
	/// マウスの座標を取得
	/// </summary>
	/// <returns>マウスの座標</returns>
	Vector2 GetMousePosition();

private:
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

	bool GetGamePadButton(GamePadButton button);

	bool GetPreGamePadButton(GamePadButton button);

	bool GetMouseButton(MouseButton button);

	bool GetPreMouseButton(MouseButton button);

private:
	WinApp* winApp_ = nullptr;

	ComPtr<IDirectInput8> dInput_;
	ComPtr<IDirectInputDevice8> devKeyboard_;
	ComPtr<IDirectInputDevice8> devMouse_;

	XINPUT_STATE xInputState_ = {};
	XINPUT_STATE preXInputState_ = {};


	BYTE key_[256] = {};
	BYTE keyPre_[256] = {};

	DIMOUSESTATE2 mouse_;
	DIMOUSESTATE2 mousePre_;
	POINT mousePos_;

};

