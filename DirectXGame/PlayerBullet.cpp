#include "PlayerBullet.h"
#include "Function.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 速度初期化
	velocity_ = velocity;
}

void PlayerBullet::Update() {

	// ワールドトランスフォームの更新←出現位置を初期値と一緒にするため
	worldTransform_.UpdateMatrix();
	
	// 速度加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}
