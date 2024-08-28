#pragma once
#include "GameScene.h"

class BaseScene {
public:
	virtual ~BaseScene() = default;

	virtual void Init(Input* input,Audio* audio) = 0;
	virtual void Update() = 0;
	virtual void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) = 0;

public:
	virtual SCENE GetNextScene() = 0;
	virtual bool GetIsExit() = 0;

protected:
	// それぞれのシーンにシーン遷移タイミングと遷移先を任せる
	SCENE NextScene;
	bool isExit;
};
