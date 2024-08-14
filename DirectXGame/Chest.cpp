#include "Chest.h"
#include <cassert>

Chest::Chest() {}

Chest::~Chest() {}

void Chest::Initialize(Model* model, const float rad) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// ワールドトランスフォーム設定
	worldTransform_.Initialize();
	//半径をセット
	rad_ = rad;
	//位置を半径に応じて変える
	worldTransform_.translation_ = {0.0f, rad_ * 2.0f, 0.0f};

}

void Chest::Update() {

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Chest::Draw(ViewProjection& viewProjection) {
	//胸部本体描画
	model_->Draw(worldTransform_, viewProjection);
}
