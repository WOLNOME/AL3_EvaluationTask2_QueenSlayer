#include "Ground.h"

void Ground::Initialize(const Vector3& position, uint32_t textureHandle) {
	//モデル生成
	model_.reset(Model::CreateFromOBJ("ground", true));
	//初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// テクスチャハンドル
	textureHandle_ = textureHandle;
}

void Ground::Update() { worldTransform_.UpdateMatrix(); }

void Ground::Draw(ViewProjection& viewProjection) {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
