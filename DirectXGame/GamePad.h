#pragma once
#include "Input.h"


class GamePad {
public:
	GamePad(Input* input);
	/// <summary>
	/// フレームの最初にインプットを取得
	/// </summary>
	/// <param name="input"></param>
	void SetCommandUpdate();

public://コマンド
	bool PushA();
	bool PushB();
	bool PushX();
	bool PushY();
	bool PushCrossUP();
	bool PushCrossDOWN();
	bool PushCrossRIGHT();
	bool PushCrossLEFT();
	bool PushRStickButton();
	bool PushRB();
	bool PushLStickButton();
	bool PushLB();
	bool PushSTART();
	bool PushBACK();

	bool TriggerA();
	bool TriggerB();
	bool TriggerX();
	bool TriggerY();
	bool TriggerCrossUP();
	bool TriggerCrossDOWN();
	bool TriggerCrossRIGHT();
	bool TriggerCrossLEFT();
	bool TriggerRStickButton();
	bool TriggerRB();
	bool TriggerLStickButton();
	bool TriggerLB();
	bool TriggerSTART();
	bool TriggerBACK();

	bool TiltLStickUP();
	bool TiltLStickUPRIGHT();
	bool TiltLStickRIGHT();
	bool TiltLStickDOWNRIGHT();
	bool TiltLStickDOWN();
	bool TiltLStickDOWNLEFT();
	bool TiltLStickLEFT();
	bool TiltLStickUPLEFT();
	bool TiltRStickUP();
	bool TiltRStickUPRIGHT();
	bool TiltRStickRIGHT();
	bool TiltRStickDOWNRIGHT();
	bool TiltRStickDOWN();
	bool TiltRStickDOWNLEFT();
	bool TiltRStickLEFT();
	bool TiltRStickUPLEFT();

	bool TriggerLStickUP();
	bool TriggerLStickUPRIGHT();
	bool TriggerLStickRIGHT();
	bool TriggerLStickDOWNRIGHT();
	bool TriggerLStickDOWN();
	bool TriggerLStickDOWNLEFT();
	bool TriggerLStickLEFT();
	bool TriggerLStickUPLEFT();
	bool TriggerRStickUP();
	bool TriggerRStickUPRIGHT();
	bool TriggerRStickRIGHT();
	bool TriggerRStickDOWNRIGHT();
	bool TriggerRStickDOWN();
	bool TriggerRStickDOWNLEFT();
	bool TriggerRStickLEFT();
	bool TriggerRStickUPLEFT();

public:
	//パッドで入力したかを伝える
	bool GetAllCommand();

private:
	//インプット
	Input* input_ = nullptr;
	//現在のコマンド
	XINPUT_STATE nowCommand;
	//1f前のコマンド
	XINPUT_STATE preCommand;


};
