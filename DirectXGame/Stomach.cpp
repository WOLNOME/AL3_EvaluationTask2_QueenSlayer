#include "Stomach.h"
#include "CollisionConfig.h"
#include "StageScene.h"
#include "TextureManager.h"
#include <cassert>

Stomach::Stomach() {}

Stomach::~Stomach() {}

void Stomach::Initialize(Model* model, const Vector3& position, const float rad) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// ワールドトランスフォーム設定
	worldTransform_.Initialize();
	// 引数で受け取った座標をセット
	worldTransform_.translation_ = position;
	// 半径をセット
	radius_ = rad;

	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定(相手の属性)
	SetCollisionMask(kCollisionAttributePlayer);
}

void Stomach::Update() {

	// プレイヤーの方向に回転
	Vector3 direction_ = Subtract(stageScene_->GetPlayer()->GetWorldPostion(), GetWorldPosition());
	direction_ = Normalize({direction_.x, 0.0f, direction_.z});
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Stomach::Draw(ViewProjection& viewProjection) {
	// 腹部本体描画

	model_->Draw(worldTransform_, viewProjection);
}

void Stomach::OnCollision() {}

void Stomach::Attack() {}

Vector3 Stomach::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
