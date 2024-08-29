#include "GameOverCamera.h"

GameOverCamera::GameOverCamera() {}

GameOverCamera::~GameOverCamera() {}

void GameOverCamera::Initialize() {
	// 座標を初期化
	lat_ = kInitPosLat_;
	lon_ = kInitPosLon_;
	// ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.x = kCenter_.x + length_ * std::cosf(lat_) * std::cosf(lon_);
	worldTransform_.translation_.y = kCenter_.y + length_ * std::sinf(lat_);
	worldTransform_.translation_.z = kCenter_.z + length_ * std::cosf(lat_) * std::sinf(lon_);
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// directionの初期化
	direction_ = Normalize(Subtract(kCenter_, worldTransform_.translation_));
}

void GameOverCamera::Update() {
	// 速度加算処理
	lat_ += velocityLat_;
	lon_ += velocityLon_;

	// ローカル座標更新
	worldTransform_.translation_.x = kCenter_.x + length_ * std::cosf(lat_) * std::cosf(lon_);
	worldTransform_.translation_.y = kCenter_.y + length_ * std::sinf(lat_);
	worldTransform_.translation_.z = kCenter_.z + length_ * std::cosf(lat_) * std::sinf(lon_);

	// カメラをディレクション方向に向けて回転
	direction_ = Normalize(Subtract(kCenter_, worldTransform_.translation_));
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 行列の更新と転送
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
}
