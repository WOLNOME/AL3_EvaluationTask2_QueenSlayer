#include "Reticle.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

Reticle::Reticle() {}

Reticle::~Reticle() {}

void Reticle::Initialize() {
	//ファイル名を指定してテクスチャを読み込む
	textureReticle_ = TextureManager::Load("target.png");
	// モデル生成
	model3DReticle_.reset(Model::CreateFromOBJ("cube", true));
	// ワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	// 2Dスプライト
	sprite2DReticle_.reset(Sprite::Create(textureReticle_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	size_ = sprite2DReticle_->GetSize();
	// ローカル座標設定
	localPosition_ = worldTransform3DReticle_.translation_;
}

void Reticle::Update(ViewProjection& viewProjection) {

	// 画面中央座標取得
	Vector2 centerPosition = {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f};
	// centerPositionを2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(centerPosition);

	// ビュープロジェクションビューポート合成行列←カメラ情報を参照
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	// スクリーン座標
	Vector3 posNear = Vector3(centerPosition.x, centerPosition.y, 0);
	Vector3 posFar = Vector3(centerPosition.x, centerPosition.y, 1);

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// センターレイの方向
	Vector3 centerDirection = Subtract(posFar, posNear);
	centerDirection = Normalize(centerDirection);

	// カメラから照準オブジェクトの距離を設定し、3Dレティクルの座標を確定
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kReticleRange_, centerDirection));

	// ローカル座標更新
	localPosition_ = worldTransform3DReticle_.translation_;
	// 3Dレティクルワールドトランスフォームの更新と転送
	worldTransform3DReticle_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("reticle");
	ImGui::Text("size : %.1f,%.1f", size_.x, size_.y);

	ImGui::End();

#endif // _DEBUG


}

void Reticle::Draw(ViewProjection& viewProjection) {
#ifdef _DEBUG
	viewProjection;
	// 3Dモデル
	//model3DReticle_->Draw(worldTransform3DReticle_, viewProjection);
#endif // _DEBUG
}

void Reticle::DrawUI() {
	// 2Dレティクル
	sprite2DReticle_->Draw();
}

const Vector3 Reticle::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}
