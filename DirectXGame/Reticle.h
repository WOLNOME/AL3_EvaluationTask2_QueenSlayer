#pragma once
#include "Function.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "WinApp.h"


class Reticle {
public:
	Reticle();
	~Reticle();

	void Initialize();
	void Update(ViewProjection& viewProjection);
	void Draw(ViewProjection& viewProjection);

public: // 関数
	void DrawUI();

public: // ゲッター
	//3Dレティクルのワールドトランスフォーム
	const WorldTransform& GetWorldTransform() { return worldTransform3DReticle_; }
	//3Dレティクルのワールド座標
	const Vector3 GetWorldPosition();
	//3Dレティクルのローカル座標
	const Vector3& GetLocalPosition() { return localPosition_; }

public: // セッター


private:
	//2Dレティクルテクスチャハンドル
	uint32_t textureReticle_;
	// 3Dレティクルワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// 3Dレティクルモデル
	std::unique_ptr<Model> model3DReticle_ = nullptr;
	// 2Dレティクルスプライト
	std::unique_ptr<Sprite> sprite2DReticle_ = nullptr;

private:
	//カメラから離す距離
	float kReticleRange_ = 35.0f;
	//ローカル座標
	Vector3 localPosition_ = {0.0f, 0.0f, 0.0f};
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// レティクル
	Vector2 size_;
};
