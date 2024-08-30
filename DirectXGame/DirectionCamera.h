#pragma once
#include "Function.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "memory"

class DirectionCamera {
public:
	DirectionCamera();
	~DirectionCamera();

	void Initialize(Vector3 enemyPos, int kMoveTime);
	void Update(int timer);

public: // 関数
public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }

public: // セッター
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

private: // 変数
	// 座標
	Vector3 pos_;
	// 初期座標
	const Vector3 initPos_ = {0.0f, 0.1f, 50.0f};
	// 目標座標
	const Vector3 goalPos_ = {0.0f, 4.0f, 20.0f};
	// 目標方向（ワールド）
	Vector3 enemyPos_;
	// カメラの向きベクトル
	Vector3 direction_;
	// 移動目標時間
	int kMoveTime_;
};
