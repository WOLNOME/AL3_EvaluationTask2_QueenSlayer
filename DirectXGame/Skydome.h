#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <memory>

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,uint32_t textureHandle);
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
	//テクスチャハンドル
	uint32_t textureHandle_;
};
