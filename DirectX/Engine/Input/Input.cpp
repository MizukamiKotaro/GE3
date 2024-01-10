#include "Input.h"
#include <cassert>

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

void Input::Initialize() {

	winApp_ = WinApp::GetInstance();

	HRESULT hr = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, nullptr);
	assert(SUCCEEDED(hr));

	hr = dInput_->CreateDevice(GUID_SysKeyboard, &devKeyboard_, NULL);
	assert(SUCCEEDED(hr));

	hr = devKeyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = devKeyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));


	hr = dInput_->CreateDevice(GUID_SysMouse, &devMouse_, NULL);
	assert(SUCCEEDED(hr));

	hr = devMouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));

	hr = devMouse_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

void Input::Update() {

	devKeyboard_->Acquire();

	memcpy(keyPre_, key_, sizeof(key_));

	devKeyboard_->GetDeviceState(sizeof(key_), key_);

	devMouse_->Acquire();

	mousePre_ = mouse_;

	devMouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouse_);

	preXInputState_ = xInputState_;
	XInputGetState(0, &xInputState_);

}

bool Input::PressingKey(BYTE keyNumber) const {
	if (key_[keyNumber] && keyPre_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::ReleasedKey(BYTE keyNumber) const
{
	if (!key_[keyNumber] && keyPre_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::PressedKey(BYTE keyNumber) const {
	if (!keyPre_[keyNumber] && key_[keyNumber]) {
		return true;
	}
	return false;
}

Vector2 Input::GetGamePadLStick()
{
	if ((xInputState_.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		xInputState_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(xInputState_.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			xInputState_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		xInputState_.Gamepad.sThumbLX = 0;
		xInputState_.Gamepad.sThumbLY = 0;
	}

	return Vector2(static_cast<float>(xInputState_.Gamepad.sThumbLX) / SHRT_MAX, static_cast<float>(xInputState_.Gamepad.sThumbLY) / SHRT_MAX);
}

Vector2 Input::GetGamePadRStick()
{
	if ((xInputState_.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		xInputState_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(xInputState_.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			xInputState_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		xInputState_.Gamepad.sThumbRX = 0;
		xInputState_.Gamepad.sThumbRY = 0;
	}

	return Vector2(static_cast<float>(xInputState_.Gamepad.sThumbRX) / SHRT_MAX, static_cast<float>(xInputState_.Gamepad.sThumbRY) / SHRT_MAX);
}

bool Input::PressedGamePadButton(GamePadButton button)
{

	if (!GetPreGamePadButton(button) && GetGamePadButton(button)) {
		return true;
	}

	return false;
}

bool Input::PressingGamePadButton(GamePadButton button)
{
	if (GetPreGamePadButton(button) && GetGamePadButton(button)) {
		return true;
	}

	return false;
}

bool Input::ReleasedGamePadButton(GamePadButton button)
{
	if (GetPreGamePadButton(button) && !GetGamePadButton(button)) {
		return true;
	}

	return false;
}

bool Input::PressedMouse(MouseButton button)
{
	if (GetMouseButton(button) && !GetPreMouseButton(button)) {
		return true;
	}

	return false;
}

bool Input::PressingMouse(MouseButton button)
{
	if (GetMouseButton(button) && GetPreMouseButton(button)) {
		return true;
	}

	return false;
}

bool Input::ReleasedMouse(MouseButton button)
{
	if (!GetMouseButton(button) && GetPreMouseButton(button)) {
		return true;
	}

	return false;
}

int32_t Input::GetWheel()
{
	return static_cast<int32_t>(mouse_.lZ);
}

Vector2 Input::GetMouseMove()
{
	return { (float)mouse_.lX,(float)mouse_.lY };
}

Vector2 Input::GetMousePosition()
{
	GetCursorPos(&mousePos_);
	ScreenToClient(WinApp::GetInstance()->GetHwnd(), &mousePos_);

	return Vector2(static_cast<float>(mousePos_.x), static_cast<float>(mousePos_.y));
}

bool Input::GetGamePadButton(GamePadButton button)
{

	switch (button)
	{
	case Input::GamePadButton::A:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			return true;
		}
		break;
	case Input::GamePadButton::B:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			return true;
		}
		break;
	case Input::GamePadButton::X:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			return true;
		}
		break;
	case Input::GamePadButton::Y:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			return true;
		}
		break;
	case Input::GamePadButton::UP:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			return true;
		}
		break;
	case Input::GamePadButton::DOWN:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			return true;
		}
		break;
	case Input::GamePadButton::LEFT:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			return true;
		}
		break;
	case Input::GamePadButton::RIGHT:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			return true;
		}
		break;
	case Input::GamePadButton::START:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
			return true;
		}
		break;
	case Input::GamePadButton::BACK:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
			return true;
		}
		break;
	case Input::GamePadButton::LEFT_THUMB:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
			return true;
		}
		break;
	case Input::GamePadButton::RIGHT_THUMB:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			return true;
		}
		break;
	case Input::GamePadButton::LEFT_SHOULDER:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			return true;
		}
		break;
	case Input::GamePadButton::RIGHT_SHOULDER:
		if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool Input::GetPreGamePadButton(GamePadButton button)
{

	switch (button)
	{
	case Input::GamePadButton::A:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			return true;
		}
		break;
	case Input::GamePadButton::B:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			return true;
		}
		break;
	case Input::GamePadButton::X:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			return true;
		}
		break;
	case Input::GamePadButton::Y:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			return true;
		}
		break;
	case Input::GamePadButton::UP:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			return true;
		}
		break;
	case Input::GamePadButton::DOWN:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			return true;
		}
		break;
	case Input::GamePadButton::LEFT:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			return true;
		}
		break;
	case Input::GamePadButton::RIGHT:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			return true;
		}
		break;
	case Input::GamePadButton::START:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
			return true;
		}
		break;
	case Input::GamePadButton::BACK:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
			return true;
		}
		break;
	case Input::GamePadButton::LEFT_THUMB:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
			return true;
		}
		break;
	case Input::GamePadButton::RIGHT_THUMB:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			return true;
		}
		break;
	case Input::GamePadButton::LEFT_SHOULDER:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			return true;
		}
		break;
	case Input::GamePadButton::RIGHT_SHOULDER:
		if (preXInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool Input::GetMouseButton(MouseButton button)
{
	uint32_t key = static_cast<uint32_t>(button);

	switch (button)
	{
	case Input::MouseButton::LEFT:
		return mouse_.rgbButtons[key];
		break;
	case Input::MouseButton::RIGHT:
		return mouse_.rgbButtons[key];
		break;
	case Input::MouseButton::CENTER:
		return mouse_.rgbButtons[key];
		break;
	default:
		break;
	}

	return false;
}

bool Input::GetPreMouseButton(MouseButton button)
{
	uint32_t key = static_cast<uint32_t>(button);

	switch (button)
	{
	case Input::MouseButton::LEFT:
		return mousePre_.rgbButtons[key];
		break;
	case Input::MouseButton::RIGHT:
		return mousePre_.rgbButtons[key];
		break;
	case Input::MouseButton::CENTER:
		return mousePre_.rgbButtons[key];
		break;
	default:
		break;
	}

	return false;
}
