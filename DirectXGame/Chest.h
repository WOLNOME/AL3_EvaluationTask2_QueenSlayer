#pragma once
#include "Collider.h"
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"

// 前方宣言
class StageScene;

class Chest : public Collider {
public:
	Chest();
	~Chest();

	void Initialize(Model* model,const float rad);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // 関数
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;
	void Attack();

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override;
	// 半径
	float GetRadius()override { return radius_; }
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	// 攻撃中コールバック
	bool GetIsAction() { return isAction_; }

public: // セッター
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;
	// テクスチャ
	uint32_t textureHandle_ = 0;
	// 初期座標
	Vector3 initPos = {0.0f, 0.0f, 0.0f};
	// 弾
	std::list<EnemyBullet*> bullets_;
	std::unique_ptr<Model> modelBullet_ = nullptr;

private:
	bool isAction_ = false;
	bool isAccumulatePower_ = true;
	bool isPreMove_ = false;
	bool isShoot_ = false;

	// 各フェーズで使うタイマー
	uint32_t timer_ = 0;
	// 各フェーズの時間
	const uint32_t accumulatePowerTime_ = 120;
	const uint32_t shootTime_ = 60;
	// 速度
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	// 振動前の座標を保持
	Vector3 prePos;
};
