#include "Stomach.h"
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
	//半径をセット
	rad_ = rad;
}

void Stomach::Update() {

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Stomach::Draw(ViewProjection& viewProjection) {
	// 腹部本体描画
	model_->Draw(worldTransform_, viewProjection);
}
