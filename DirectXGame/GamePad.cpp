#include "GamePad.h"
#include "Function.h"

GamePad::GamePad(Input* input) { input_ = input; }

void GamePad::SetCommandUpdate() {
	// 現在
	input_->GetJoystickState(0, nowCommand);
	// 1f前
	input_->GetJoystickStatePrevious(0, preCommand);
}

bool GamePad::PushA() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushB() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushX() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_X) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushY() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_Y) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushCrossUP() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushCrossDOWN() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushCrossRIGHT() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushCrossLEFT() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushRStickButton() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_THUMB) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushRB() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushLStickButton() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushLB() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushSTART() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_START) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::PushBACK() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_BACK) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerA() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_A && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_A) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerB() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_B && preCommand.Gamepad.wButtons != !XINPUT_GAMEPAD_B) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerX() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_X && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_X) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerY() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_Y && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_Y) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerCrossUP() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_UP) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerCrossDOWN() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_DOWN) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerCrossRIGHT() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_RIGHT) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerCrossLEFT() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_DPAD_LEFT) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerRStickButton() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_THUMB && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_RIGHT_THUMB) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerRB() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerLStickButton() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_LEFT_THUMB) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerLB() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_LEFT_SHOULDER) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerSTART() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_START && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_START) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TriggerBACK() {
	if (nowCommand.Gamepad.wButtons == XINPUT_GAMEPAD_BACK && preCommand.Gamepad.wButtons != XINPUT_GAMEPAD_BACK) {
		return true;
	} else {
		return false;
	}
}

bool GamePad::TiltLStickUP() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(22.5f / 180.0f) * pi && theta <= (22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltLStickUPRIGHT() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (22.5f / 180.0f) * pi && theta <= (67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltLStickRIGHT() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (67.5f / 180.0f) * pi && theta <= (112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltLStickDOWNRIGHT() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (112.5f / 180.0f) * pi && theta <= (157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltLStickDOWN() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (157.5f / 180.0f) * pi || theta <= -(157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltLStickDOWNLEFT() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(157.5f / 180.0f) * pi && theta <= -(112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltLStickLEFT() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(112.5f / 180.0f) * pi && theta <= -(67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltLStickUPLEFT() {
	if (nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(67.5f / 180.0f) * pi && theta <= -(22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickUP() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(22.5f / 180.0f) * pi && theta <= (22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickUPRIGHT() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (22.5f / 180.0f) * pi && theta <= (67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickRIGHT() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (67.5f / 180.0f) * pi && theta <= (112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickDOWNRIGHT() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (112.5f / 180.0f) * pi && theta <= (157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickDOWN() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (157.5f / 180.0f) * pi || theta <= -(157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickDOWNLEFT() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(157.5f / 180.0f) * pi && theta <= -(112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickLEFT() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(112.5f / 180.0f) * pi && theta <= -(67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TiltRStickUPLEFT() {
	if (nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(67.5f / 180.0f) * pi && theta <= -(22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickUP() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(22.5f / 180.0f) * pi && theta <= (22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickUPRIGHT() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (22.5f / 180.0f) * pi && theta <= (67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickRIGHT() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (67.5f / 180.0f) * pi && theta <= (112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickDOWNRIGHT() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (112.5f / 180.0f) * pi && theta <= (157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickDOWN() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (157.5f / 180.0f) * pi || theta <= -(157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickDOWNLEFT() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(157.5f / 180.0f) * pi && theta <= -(112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickLEFT() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(112.5f / 180.0f) * pi && theta <= -(67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerLStickUPLEFT() {
	if ((nowCommand.Gamepad.sThumbLX || nowCommand.Gamepad.sThumbLY) && (!preCommand.Gamepad.sThumbLX && !preCommand.Gamepad.sThumbLY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbLX, (float)nowCommand.Gamepad.sThumbLY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(67.5f / 180.0f) * pi && theta <= -(22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickUP() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(22.5f / 180.0f) * pi && theta <= (22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickUPRIGHT() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (22.5f / 180.0f) * pi && theta <= (67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickRIGHT() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (67.5f / 180.0f) * pi && theta <= (112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickDOWNRIGHT() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (112.5f / 180.0f) * pi && theta <= (157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickDOWN() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= (157.5f / 180.0f) * pi || theta <= -(157.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickDOWNLEFT() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(157.5f / 180.0f) * pi && theta <= -(112.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickLEFT() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(112.5f / 180.0f) * pi && theta <= -(67.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::TriggerRStickUPLEFT() {
	if ((nowCommand.Gamepad.sThumbRX || nowCommand.Gamepad.sThumbRY) && (!preCommand.Gamepad.sThumbRX && !preCommand.Gamepad.sThumbRY)) {
		// 基準ベクトル
		Vector2 standardVector = Normalize(Vector2(0, 1.0f));
		// 入力ベクトル
		Vector2 inputVector = Normalize(Vector2((float)nowCommand.Gamepad.sThumbRX, (float)nowCommand.Gamepad.sThumbRY));
		// 角度
		float theta = AngleOf2Vector(standardVector, inputVector);
		// 角度に対応した処理
		if (theta >= -(67.5f / 180.0f) * pi && theta <= -(22.5f / 180.0f) * pi) {
			return true;
		} else {
			false;
		}
	}
	return false;
}

bool GamePad::GetIsInput() {
	if (
		nowCommand.Gamepad.wButtons||
		nowCommand.Gamepad.sThumbLX||
		nowCommand.Gamepad.sThumbLY||
		nowCommand.Gamepad.sThumbRX||
		nowCommand.Gamepad.sThumbRY
		) {
		return true;
	} else {
		return false;
	}
}
