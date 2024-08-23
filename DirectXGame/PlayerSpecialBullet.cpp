#include "PlayerSpecialBullet.h"
#include "Function.h"
#include "CollisionConfig.h"
#include <cassert>

PlayerSpecialBullet::PlayerSpecialBullet() {}

PlayerSpecialBullet::~PlayerSpecialBullet() {}

void PlayerSpecialBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 速度初期化
	velocity_ = velocity;
	// 半径設定
	radius_ = 1.0f;
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributePlayerSpecialBullet);
}

void PlayerSpecialBullet::Update() {
	// ワールドトランスフォームの更新←出現位置を初期値と一緒にするため
	worldTransform_.UpdateMatrix();
	
	//重力加算
	velocity_.y += kGravity_.y;
	// 速度加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerSpecialBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void PlayerSpecialBullet::OnCollision() {}

void PlayerSpecialBullet::OnSpecialCollision() { isDead_ = true; }

Vector3 PlayerSpecialBullet::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
