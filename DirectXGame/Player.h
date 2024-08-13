#pragma once
#include "Function.h"
#include "Model.h"
#include "ShootingStand.h"
#include "WorldTransform.h"
#include "Input.h"

// 前方宣言
class StageScene;

class Player {
public:
	Player();
	~Player();

	void Initialize(Model* model, const Vector3& position, Input* input);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

public: // セッター
	void SetGameScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	// 入力
	Input* input_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;
	// 砲台
	std::unique_ptr<ShootingStand> stand_ = nullptr;
	std::unique_ptr<Model> modelStand_ = nullptr;

private: // 調整可能数値
	const float speed_ = 0.2f;
};
