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

public://関数
	void Attack();

public: // ゲッター
	const Vector3 GetWorldPostion() { return vehicle_->GetWorldPostion(); }

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

};
