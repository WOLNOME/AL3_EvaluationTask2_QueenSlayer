#include "Background.h"

Background::Background() {}

Background::~Background() {}

void Background::Initialize(const Vector3& position) {
	//モデルの生成
	modelLeaf_.reset(Model::CreateFromOBJ("leafBG", true));
	modelWood_.reset(Model::CreateFromOBJ("woodBG", true));
	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void Background::Update() {}

void Background::Draw(ViewProjection& viewProjection) {
	// 木モデルの描画
	modelWood_->Draw(worldTransform_, viewProjection);
	//葉モデルの描画
	modelLeaf_->Draw(worldTransform_, viewProjection);
}
