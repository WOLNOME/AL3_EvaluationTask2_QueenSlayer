#pragma once
#include "Input.h"
#include "Sprite.h"
#include "Function.h"
#include "memory"

//現在の状態
enum PauseSituation {
	MENU,
	HOWTOPLAY,
};
//メニュー画面にあるボタン
enum MenuButton {
	CONTINUEBUTTON,
	HOWTOPLAYBUTTON,
	BACKTOTITLEBUTTON,
};

class Pause {
public:
	Pause(Input* input);
	~Pause();

	void Initialize();
	void Update();
	void Draw();
	void DrawUI();
	
public://関数
public://ゲッター
	bool GetIsContinue() { return isContinue_; }
	bool GetIsBackToTitle() { return isExit_; }

public://セッター
private:
	//インプット
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandleContinueButton_;
	uint32_t textureHandleHowToPlayButton_;
	uint32_t textureHandleBackToTitleButton_;
	uint32_t textureHandleBackBlack_;
	uint32_t textureHandlePauseMark_;
	uint32_t textureHandleHTPSlide_[4];

	//スプライト
	std::unique_ptr<Sprite> spriteContinueButton_;
	std::unique_ptr<Sprite> spriteHowToPlayButton_;
	std::unique_ptr<Sprite> spriteBackToTitleButton_;
	std::unique_ptr<Sprite> spriteBackBlack_;
	std::unique_ptr<Sprite> spritePauseMark_;
	std::unique_ptr<Sprite> spriteHTPSlide_[4];

	//ボタンのサイズ
	Vector2 sizeContinueButton_;
	Vector2 sizeHowToPlayButton_;
	Vector2 sizeBackToTitleButton_;

private:
	PauseSituation pauseSituation_;
	MenuButton selectButton_;
	bool isContinue_;
	bool isExit_;
	int slidePage_;
	const float kBlackBackAlpha_ = 0.5f;
	const float kStrngthButtonRatio_ = 1.2f;

};
