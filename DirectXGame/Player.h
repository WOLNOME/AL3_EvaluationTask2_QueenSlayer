#pragma once
#include "Function.h"
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "PlayerSpecialBullet.h"
#include "ShootingStand.h"
#include "Vehicle.h"
#include "WorldTransform.h"

// 前方宣言
class StageScene;

class Player {
public:
	Player();
	~Player();

	void Initialize(const Vector3& position, Input* input);
	void Update();
	void Draw(ViewProjection& viewProjection);

private://関数
	void Attack();
	void Damage();
	void Invincible();

public: // ゲッター
	const Vector3 GetWorldPostion() { return vehicle_->GetWorldPosition(); }
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	// 必殺弾リストを取得
	const std::list<PlayerSpecialBullet*>& GetSpecialBullets() const { return specialBullets_; }
	//車両部獲得
	const std::unique_ptr<Vehicle>& GetVehicle() const { return vehicle_; }
	// 砲台部獲得
	const std::unique_ptr<ShootingStand>& GetStand() const { return stand_; }
	//死亡判定
	bool GetIsDead() { return isDead_; }
	//HP獲得
	int GetHP() { return nowHP_; }
	// HP獲得
	int GetSPGauge() { return nowSPGauge_; }


public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }


public:
	//最大体力
	const int kMaxHP_ = 8;
	const int kMaxSPGauge_ = 7;



private:
	// 入力
	Input* input_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;
	// 車両
	std::unique_ptr<Vehicle> vehicle_ = nullptr;
	std::unique_ptr<Model> modelVehicle_ = nullptr;
	// 砲台
	std::unique_ptr<ShootingStand> stand_ = nullptr;
	std::unique_ptr<Model> modelStand_ = nullptr;
	// 通常弾
	std::list<PlayerBullet*> bullets_;
	std::unique_ptr<Model> modelBullet_ = nullptr;
	// 必殺弾
	std::list<PlayerSpecialBullet*> specialBullets_;
	std::unique_ptr<Model> modelSpecialBullet_ = nullptr;


private:
	//弾の速度
	const float kBulletSpeed_ = 1.0f;
	//連続弾インターバル
	const int kBulletInterval_ = 8;
	int interval_ = 0;

	//プレイヤーのHP
	int nowHP_;
	//プレイヤーのSP関連
	int nowSPGauge_;
	//SP射出可能フラグ
	bool isUseSP_ = false;
	//死亡フラグ
	bool isDead_;

	//無敵時間
	const int kInviTime_ = 60 * 2;
	int nowInviTimer_;
	bool isInvincible_;
	bool isDisplay_;
};
