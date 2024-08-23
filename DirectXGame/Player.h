#pragma once
#include "Function.h"
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "ShootingStand.h"
#include "Vehicle.h"
#include "WorldTransform.h"

// 前方宣言
class StageScene;

class Player {
public:
	Player();
	~Player();

	void Initialize(const Vector3& position, Input* input);
	void Update();
	void Draw(ViewProjection& viewProjection);

private://関数
	void Attack();
	void Damage();

public: // ゲッター
	const Vector3 GetWorldPostion() { return vehicle_->GetWorldPosition(); }
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	//車両部獲得
	const std::unique_ptr<Vehicle>& GetVehicle() const { return vehicle_; }
	// 砲台部獲得
	const std::unique_ptr<ShootingStand>& GetStand() const { return stand_; }
	//死亡判定
	bool GetIsDead() { return isDead_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	// 入力
	Input* input_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;
	// 車両
	std::unique_ptr<Vehicle> vehicle_ = nullptr;
	std::unique_ptr<Model> modelVehicle_ = nullptr;
	// 砲台
	std::unique_ptr<ShootingStand> stand_ = nullptr;
	std::unique_ptr<Model> modelStand_ = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;
	std::unique_ptr<Model> modelBullet_ = nullptr;

private:
	//弾の速度
	const float kBulletSpeed_ = 1.0f;
	//連続弾インターバル
	const uint32_t kBulletInterval_ = 8;
	uint32_t interval_ = 0;

	//プレイヤーのHP
	const uint32_t kMaxHP_ = 20;
	uint32_t nowHP_;
	//プレイヤーのSPゲージ
	const uint32_t kMaxSP_ = 10;
	uint32_t nowSP_;
	//死亡フラグ
	bool isDead_;
};
