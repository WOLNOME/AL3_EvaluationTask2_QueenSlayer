#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Audio.h"

class ResultScene : public BaseScene {
public:
	ResultScene();
	~ResultScene() override;

public:
	void Init(Input* input, Audio* audio) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public:
	SCENE GetNextScene() override { return NextScene; }
	bool GetIsExit() override { return isExit; }

private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

private:
	// BGM関連
	uint32_t soundHandleBGM_;
	uint32_t voiceHandleBGM_;
	bool isSoundPlayBGM_ = false;
	const float soundVolumeBGM_ = 0.7f;
};
