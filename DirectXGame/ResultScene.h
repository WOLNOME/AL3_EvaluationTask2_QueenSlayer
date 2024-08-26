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

private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
};
