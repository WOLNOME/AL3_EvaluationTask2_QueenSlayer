#include "TPSCamera.h"
#include "Function.h"
#include "StageScene.h"
#include <imgui.h>

void TPSCamera::Initialize(Input* input) {
	// 入力の設定
	input_ = input;
	// ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.x = toCenterDirection.x + length_ * cosf(lat) * cosf(lon);
	worldTransform_.translation_.y = toCenterDirection.y + length_ * sinf(lat);
	worldTransform_.translation_.z = toCenterDirection.z + length_ * cosf(lat) * sinf(lon);
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	//directionの初期化
	direction_ = Normalize(Subtract(toCenterDirection, worldTransform_.translation_));
}

void TPSCamera::Update() {
	// 移動の加算
	if (input_->PushKey(DIK_UP)) {
		lat -= Speed_;
	}
	if (input_->PushKey(DIK_DOWN)) {
		lat += Speed_;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		lon -= Speed_;
	}
	if (input_->PushKey(DIK_LEFT)) {
		lon += Speed_;
	}
	//移動制限
	lat = max(lat, -1.0f / 10.0f * pi);
	lat = min(lat, 1.0f / 15.0f * pi);
	//座標の更新
	worldTransform_.translation_.x = toCenterDirection.x + length_ * cosf(lat) * cosf(lon);
	worldTransform_.translation_.y = toCenterDirection.y + length_ * sinf(lat);
	worldTransform_.translation_.z = toCenterDirection.z + length_ * cosf(lat) * sinf(lon);

	// 回転の加算(座標を基に)
	direction_ = Normalize(Subtract(toCenterDirection, worldTransform_.translation_));
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	//行列の再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//プレイヤーの座標だけを掛ける
	worldTransform_.matWorld_ = Multiply(worldTransform_.matWorld_, MakeTranslateMatrix(stageScene_->GetPlayer()->GetWorldPostion()));
	// 行列の転送
	worldTransform_.TransferMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

#ifdef _DEBUG
	ImGui::Begin("debugTPSCamera");
	ImGui::Text("lat(y):%f,lon(x):%f", lat, lon);
	ImGui::End();

#endif // _DEBUG
}
