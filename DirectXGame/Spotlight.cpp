#include "Spotlight.h"
#include "ImGuiManager.h"

void Spotlight::Initialize(const Vector3& position) {
	// モデル生成
	model_.reset(Model::CreateFromOBJ("spotlight", true));
	// 初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//α
	model_->SetAlpha(alpha_);
}

void Spotlight::Update() { 
	worldTransform_.UpdateMatrix();
#ifdef _DEBUG
	ImGui::Begin("spotlight");
	ImGui::SliderFloat("alpha", &alpha_, 0.0f, 1.0f);
	ImGui::End();
#endif // _DEBUG
}

void Spotlight::Draw(ViewProjection& viewProjection) {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection);
}
