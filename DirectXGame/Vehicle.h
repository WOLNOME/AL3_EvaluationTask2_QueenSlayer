#pragma once
#include "Collider.h"
#include "Function.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class StageScene;

class Vehicle : public Collider {
public:
	Vehicle();
	~Vehicle();

	void Initialize(Input* input,Model* model,const Vector3& position,UseScene useScene);
	void Update();
	void Draw(const ViewProjection& viewProjection);


public: // 関数
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;
	//オブジェクトとの衝突デノミ呼び出されるコールバック関数
	void OnSpecialCollision() override;


public://ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	//ローカル座標
	const Vector3& GetLocalPosition() { return localPos_; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override;
	// 半径
	float GetRadius() { return rad_; }
	// 被弾ゲッター
	bool GetIsDamage() { return isDamage_; }
	// 光玉ゲッター
	bool GetIsGetShineBall() { return isGetShineBall_; }

public://セッター
	//ステージシーンのセット
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }
	void SetIsDamage(bool isDamage) { isDamage_ = isDamage; }
	void SetIsGetShineBall(bool isGetShineBall) { isGetShineBall_ = isGetShineBall; }

private:
	//入力
	Input* input_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_;
	//ステージシーン
	StageScene* stageScene_ = nullptr;
	//使われるシーン
	UseScene useScene_;


private: // 調整可能数値
	// スピード
	const float speed_ = 0.2f;
	// 移動方向の傾き
	float radian_ = 0.0f;
	// プレイヤーの向きベクトル
	Vector3 direction_ = {0.0f, 1.0f, 0.0f};
	// 移動判定変数
	bool isMove = false;
	// プレイヤーの回転速度
	const float rotateSpeed_ = (2.0f / 2.0f) * pi / 60.0f;
	//ローカル座標保存用
	Vector3 localPos_ = {0.0f, 0.0f, 0.0f};
	//半径
	const float rad_ = 1.25f;
	// ダメージ蓄積フラグ
	bool isDamage_ = false;
	// 光玉獲得フラグ
	bool isGetShineBall_ = false;
	// カメラの向き
	Vector3 cameraDir = {0.0f, 0.0f, 0.0f};


};
