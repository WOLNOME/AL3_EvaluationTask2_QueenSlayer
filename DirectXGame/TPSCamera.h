#pragma once
#include "Function.h"
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cmath>

//前方宣言
class StageScene;

class TPSCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const Vector3& GetDirectionToPlayer() { return direction_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	// 入力
	Input* input_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// プレイヤーとの距離
	const float length_ = 10.0f;
	// 視点移動速度
	const float Speed_ = (1.0f / 3.0f * pi) / 60.0f;
	// 現在の視点角度座標
	float lat = 1.0f / 18.0f * pi;
	float lon = 1.0f / 2.0f * pi;
	// カメラからプレイヤーへの方向
	Vector3 direction_ = {0.0f, 0.0f, 0.0f};
	// カメラの視点座標(カメラの周る球の中心座標/本来はプレイヤーが原点)
	Vector3 toCenterDirection = {0.0f, 3.0f, 0.0f};

	//ステージシーン
	StageScene* stageScene_ = nullptr;

};
