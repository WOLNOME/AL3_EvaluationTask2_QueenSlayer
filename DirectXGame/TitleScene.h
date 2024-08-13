#pragma once
#include "BaseScene.h"
#include "Input.h"
class TitleScene : public BaseScene
{
public:
	TitleScene();
	~TitleScene()override;
public:
	void Init(Input* input) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public:
	SCENE GetNextScene()override { return NextScene; }
private:
	Input* input_ = nullptr;

};

