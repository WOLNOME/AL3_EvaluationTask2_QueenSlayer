#pragma once
#include "Collider.h"
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"

// 前方宣言
class StageScene;

class Stomach : public Collider {
public:
	Stomach();
	~Stomach();

	void Initialize(Model* model, const Vector3& position, const float rad);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // 関数
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;
	void Attack();
	void Jump();

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override;
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	// 半径
	float GetRadius() override { return radius_; }
	// 攻撃中コールバック
	bool GetIsAction() { return isAction_; }
	// 光玉蓄積許可
	bool GetIsHit() { return isHit_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }
	void SetIsHit(bool isHit) { isHit_ = isHit; }

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
	bool isJump_ = false;

	// 各フェーズで使うタイマー
	uint32_t timer_ = 0;
	// 各フェーズの時間
	const uint32_t accumulatePowerTime_ = 120;
	const uint32_t shootTime_ = 60;
	//速度
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	//最大縮小値
	const float kReducation_ = 0.7f;
	//振動前の座標を保持
	Vector3 prePos;
	// 光玉蓄積フラグ
	bool isHit_ = false;

};
