#pragma once
#include "Chest.h"
#include "Function.h"
#include "Head.h"
#include "Input.h"
#include "Model.h"
#include "Stomach.h"
#include "WorldTransform.h"

// 行動パターン
enum EnemyActionPattern {
	Stop,
	AttackHead,
	AttackChest,
	AttackStomach,
	Move,
	kAllActionNum,
};

// 前方宣言
class StageScene;

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // ゲッター
	// 腹部獲得
	const std::unique_ptr<Stomach>& GetStomach() const { return stomach_; }
	// 胸部獲得
	const std::unique_ptr<Chest>& GetChest() const { return chest_; }
	// 頭部獲得
	const std::unique_ptr<Head>& GetHead() const { return head_; }
	// HP獲得
	uint32_t GetHP() { return nowHP_; }
	// SP獲得
	uint32_t GetSP() { return nowSP_; }
	// 死亡判定
	bool GetIsDead() { return isDead_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }
	void SetSP(uint32_t newSP) { nowSP_ = newSP; }

private:
	// STOPアクション
	void StopAction();
	// 次のアクションルーレット
	EnemyActionPattern ActionRoulette();
	// 被弾処理
	void Damage();

public: // 定数
	//最大HP
	const uint32_t kMaxHP_ = 1200;

private:
	// 入力
	Input* input_ = nullptr;
	// 腹部
	std::unique_ptr<Stomach> stomach_ = nullptr;
	std::unique_ptr<Model> modelStomach_ = nullptr;
	// 胸部
	std::unique_ptr<Chest> chest_ = nullptr;
	std::unique_ptr<Model> modelChest_ = nullptr;
	// 頭部
	std::unique_ptr<Head> head_ = nullptr;
	std::unique_ptr<Model> modelHead_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;

private:
	// 各部位の半径
	const float rad_ = 1.5f;
	// 行動
	EnemyActionPattern action_ = Stop;

	// アクション中フラグ
	bool isAction_ = true;
	// アクションタイマー
	uint32_t timer_ = 0;
	// ストップタイマー制限時間
	const uint32_t kStopTime_ = 140;

	// 敵の体力
	uint32_t nowHP_;
	const uint32_t kDamageSmall_ = 1;
	const uint32_t kDamageMedium_ = 3;
	const uint32_t kDamageLarge_ = 120;

	// 光玉蓄積値
	uint32_t nowSP_;

	// 死亡フラグ
	bool isDead_;
};
