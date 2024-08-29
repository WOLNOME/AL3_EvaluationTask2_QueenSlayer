#include "Skydome.h"

void Skydome::Initialize(const Vector3& position,uint32_t textureHandle) {
	//モデル生成
	model_.reset(Model::CreateFromOBJ("skydome", true));
	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//テクスチャハンドル
	textureHandle_ = textureHandle;
}

void Skydome::Update() { worldTransform_.UpdateMatrix(); }

void Skydome::Draw(ViewProjection& viewProjection) {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}
