#pragma once
#include "Input.h"
#include "Sprite.h"
#include "Function.h"
#include "memory"

// 現在の状態
enum TitleSituation {
	TITLEMODE,
	HOWTOPLAYMODE,
};
// タイトル画面にあるボタン
enum TitleButton {
	PLAYBUTTONTITLE,
	HOWTOPLAYBUTTONTITLE,
	EXITBUTTONTITLE,
};

class TitleUI {
public:
	TitleUI(Input* input);
	~TitleUI();

	void Initialize();
	void Update();
	void Draw();
	void DrawUI();

public: // 関数
public: // ゲッター
	bool GetIsPlay() { return isPlay_; }
	bool GetIsExit() { return isExit_; }

public: // セッター
private:
	// インプット
	Input* input_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandlePlayButton_;
	uint32_t textureHandleHowToPlayButton_;
	uint32_t textureHandleExitButton_;
	uint32_t textureHandleBackWhite_;
	uint32_t textureHandleTitleRogo_;
	uint32_t textureHandleHTPSlide_[4];

	// スプライト
	std::unique_ptr<Sprite> spritePlayButton_;
	std::unique_ptr<Sprite> spriteHowToPlayButton_;
	std::unique_ptr<Sprite> spriteExitButton_;
	std::unique_ptr<Sprite> spriteBackWhite_;
	std::unique_ptr<Sprite> spriteTitleRogo_;
	std::unique_ptr<Sprite> spriteHTPSlide_[4];

	// サイズ
	Vector2 sizePlayButton_;
	Vector2 sizeHowToPlayButton_;
	Vector2 sizeExitButton_;

	// 回転
	float rotationTitleRogo_;

private:
	TitleSituation titleSituation_;
	TitleButton selectButton_;
	bool isPlay_;
	bool isExit_;
	int slidePage_;
	const float kWhiteBackAlpha_ = 0.15f;
	const float kStrngthButtonRatio_ = 1.2f;
	const float kRotateSpeed_ = (7.0f / 180.0f) * pi * (1.0f / 150.0f);
	float rotateSpeed_;
	float rotateAmount_;
};
