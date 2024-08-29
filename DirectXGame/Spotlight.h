#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <memory>

class Spotlight {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// テクスチャ
	uint32_t textureHandle_;

private:
	float alpha_ = 1.0f;

};
