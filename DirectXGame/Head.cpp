#include "Head.h"
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
	rad_ = rad;
	// 位置を半径に応じて変える
	worldTransform_.translation_ = {0.0f, rad_ * 2.0f, 0.0f};
}

void Head::Update() {

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Head::Draw(ViewProjection& viewProjection) {
	//頭部本体描画
	model_->Draw(worldTransform_, viewProjection);
}
