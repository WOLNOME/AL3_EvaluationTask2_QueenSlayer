#include "DirectionCamera.h"
#include <cmath>

DirectionCamera::DirectionCamera() {}

DirectionCamera::~DirectionCamera() {}

void DirectionCamera::Initialize(Vector3 enemyPos,int kMoveTime) {
	//座標初期化
	pos_ = initPos_;
	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos_;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	//目標方向初期化
	enemyPos_ = enemyPos;
	//移動目標時間
	kMoveTime_ = kMoveTime;
	// directionの初期化
	direction_ = Normalize(Subtract(enemyPos_, worldTransform_.translation_));

}

void DirectionCamera::Update(int timer) {
	//座標の移動
	if (timer <= kMoveTime_) {
		worldTransform_.translation_ = Lerp(initPos_, goalPos_, ((float)timer / kMoveTime_));
	}
	if (timer >= kMoveTime_) {
		worldTransform_.translation_ = goalPos_;
	}
	// カメラをディレクション方向に向けて回転
	direction_ = Normalize(Subtract(enemyPos_, worldTransform_.translation_));
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 行列の更新と転送
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
}
