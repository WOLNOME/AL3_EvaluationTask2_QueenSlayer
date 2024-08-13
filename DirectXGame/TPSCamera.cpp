#include "TPSCamera.h"
#include "Function.h"
#include <imgui.h>

void TPSCamera::Initialize(Input* input) {
	// 入力の設定
	input_ = input;
	// ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.x = length_ * cosf(lat) * cosf(lon);
	worldTransform_.translation_.y = length_ * sinf(lat);
	worldTransform_.translation_.z = length_ * cosf(lat) * sinf(lon);
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void TPSCamera::Update() {
	// 移動の加算
	if (input_->PushKey(DIK_UP)) {
		lat += Speed_;
	}
	if (input_->PushKey(DIK_DOWN)) {
		lat -= Speed_;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		lon += Speed_;
	}
	if (input_->PushKey(DIK_LEFT)) {
		lon -= Speed_;
	}

	lat = max(lat, -1.0f / 120.0f * pi);
	lat = min(lat, 1.0f / 6.0f * pi);

	worldTransform_.translation_.x = length_ * cosf(lat) * cosf(lon);
	worldTransform_.translation_.y = length_ * sinf(lat);
	worldTransform_.translation_.z = length_ * cosf(lat) * sinf(lon);

	// 回転の加算(座標を基に)
	direction_ = Normalize(Subtract(toCenterDirection, worldTransform_.translation_));
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 行列の更新
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

#ifdef _DEBUG
	ImGui::Begin("debugTPSCamera");
	ImGui::Text("lat(y):%f,lon(x):%f", lat, lon);
	ImGui::End();

#endif // _DEBUG
}

void TPSCamera::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
