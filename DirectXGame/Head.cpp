#include "Head.h"
#include "CollisionConfig.h"
#include <cassert>

Head::Head() {}

Head::~Head() {}

void Head::Initialize(Model* model, const float rad) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// ワールドトランスフォーム設定
	worldTransform_.Initialize();
	// 半径をセット
	radius_ = rad;
	// 位置を半径に応じて変える
	worldTransform_.translation_ = {0.0f, radius_ * 2.0f, 0.0f};
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定(相手の属性)
	SetCollisionMask(kCollisionAttributePlayer);
}

void Head::Update() {

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Head::Draw(ViewProjection& viewProjection) {
	//頭部本体描画
	model_->Draw(worldTransform_, viewProjection);
}

void Head::OnCollision() {}

void Head::Attack() {}

Vector3 Head::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
