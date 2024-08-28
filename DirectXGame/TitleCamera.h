#pragma once
#include "Function.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "memory"
#include <cmath>

// カメラ移動するコース
enum MoveCourse {
	COURCEONE,
	COURCETWO,
	COURCETHREE,
	COURCEFOUR,
	kMaxCourceNum,
};

class TitleCamera {
public:
	TitleCamera();
	~TitleCamera();

	void Initialize();
	void Update();

public: // 関数
public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }

public: // セッター
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

private: // 変数
	// 1コースを周る時間
	const int kTimeOfOneCource = 60 * 15;
	// タイマー
	int timerOfOneCource;
	// 現在のコース
	MoveCourse nowCource;
	// 各コースでの半径距離
	const float length_[MoveCourse::kMaxCourceNum] = {20.0f, 25.0f, 40.0f, 100.0f};
	// 球面の座標
	float lat_;
	float lon_;
	// コーススタート座標
	const float startLat_[MoveCourse::kMaxCourceNum] = {-(10.0f / 180.0f) * pi, -(10.0f / 180.0f) * pi, (30.0f / 180.0f) * pi, (35.0f / 180.0f) * pi};
	const float startLon_[MoveCourse::kMaxCourceNum] = {(60.0f / 180.0f) * pi, (90.0f / 180.0f) * pi, -(90.0f / 180.0f) * pi, (110.0f / 180.0f) * pi};
	// コースゴール座標
	const float goalLat_[MoveCourse::kMaxCourceNum] = {(30.0f / 180.0f) * pi, (40.0f / 180.0f) * pi, (30.0f / 180.0f) * pi, (40.0f / 180.0f) * pi};
	const float goalLon_[MoveCourse::kMaxCourceNum] = {(120.0f / 180.0f) * pi, (90.0f / 180.0f) * pi, -(150.0f / 180.0f) * pi, (60.0f / 180.0f) * pi};
	// 球面速度
	float velocityLat_;
	float velocityLon_;
	// カメラの向きベクトル
	Vector3 direction_;

	// 原点(敵の中心)
	const Vector3 kCenter_ = {10.0f, 0.0f, 0.0f};
};
