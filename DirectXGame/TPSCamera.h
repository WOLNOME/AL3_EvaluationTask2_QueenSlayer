#pragma once
#include "Function.h"
#include "Input.h"
#include "GamePad.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Sprite.h"
#include "memory"
#include <cmath>

//ロックオンできる部位
enum Parts {
	HEAD,
	CHEST,
	STOMACH,
};

//前方宣言
class StageScene;

class TPSCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input, GamePad* pad);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	void DrawUI();

public://関数
	void LockOnCameraProcess();
	void NormalCameraProcess();

public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const Vector3 GetWorldPosition();
	const Vector3& GetDirectionToPlayer() { return direction_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	// 入力
	Input* input_ = nullptr;
	GamePad* pad_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	//ステージシーン
	StageScene* stageScene_ = nullptr;
	//ロックオンターゲットスプライト
	std::unique_ptr<Sprite> spriteLockOnTarget_ = nullptr;
	//スプライトのテクスチャハンドル
	uint32_t textureHandleLockOnTarget_;
	//スプライトのサイズ取得
	Vector2 lockOnTargetSize_;

private://調整変数
	// カメラからプレイヤーへの方向
	Vector3 direction_ = {0.0f, 0.0f, 0.0f};
	// カメラの視点座標(カメラの周る球の中心座標/本来はプレイヤーが原点)
	Vector3 toCenterDirectionLocal_ = {0.0f, 3.0f, 0.0f};
	Vector3 toCenterDirectionWorld_;
	// プレイヤーとの距離
	const float length_ = 10.0f;
	// 視点移動速度
	const float kNormalSpeed_ = (120.0f / 360.0f * pi) / 60.0f;
	float speedLat_ = kNormalSpeed_;
	float speedLon_ = kNormalSpeed_;
	// 現在の視点角度座標
	float lat = 1.0f / 18.0f * pi;//緯度
	float lon = 1.0f / 2.0f * pi;//経度
	// ロックオン処理変数
	bool isLockOn = false;//ロックオン状態か
	bool isTransition = false;//ロックオン遷移状態か
	// ロックオン遷移フレーム
	const int kTransitionFrame = 6;
	//遷移カウントダウン
	int countTransition_ = 0;
	//ロックオン中の部位
	Parts lockOnParts;
	//ロックオンUIの最終的なサイズ比
	const float kLockOnTargetGoalSize_ = 1.0f / 8.0f;
};
