#include "Player.h"
#include "ImGuiManager.h"
#include "StageScene.h"
#include <cassert>

Player::Player() {}

Player::~Player() {
	// 解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	for (PlayerSpecialBullet* bullet : specialBullets_) {
		delete bullet;
	}
}

void Player::Initialize(const Vector3& position, Input* input) {
	// 入力
	input_ = input;
	// 車両モデルの生成
	modelVehicle_.reset(Model::CreateFromOBJ("playerUnder", true));
	// 砲台モデルの生成
	modelStand_.reset(Model::CreateFromOBJ("playerAbove", true));
	// 弾モデルの生成
	modelBullet_.reset(Model::CreateFromOBJ("playerBullet", true));
	// 必殺弾モデルの生成
	modelSpecialBullet_.reset(Model::CreateFromOBJ("playerSpecialBullet", true));
	// 車両の生成
	vehicle_ = std::make_unique<Vehicle>();
	// 車両にstageSceneをセット
	vehicle_->SetStageScene(stageScene_);
	// 車両の初期化
	vehicle_->Initialize(input_, modelVehicle_.get(), position);
	// 砲台の生成
	stand_ = std::make_unique<ShootingStand>();
	// 砲台の初期化
	stand_->Initialize(modelStand_.get());

	/// パラメーター
	nowHP_ = kMaxHP_;
	nowSPGauge_ = 0;
	nowSPNum_ = 0;
	isDead_ = false;
}

void Player::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	specialBullets_.remove_if([](PlayerSpecialBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 攻撃処理
	Attack();

	// 車両の更新
	vehicle_->Update();
	// 砲台の更新
	Vector3 cameraDir = {stageScene_->GetTPSCamera()->GetDirectionToPlayer().x, 0.0f, stageScene_->GetTPSCamera()->GetDirectionToPlayer().z};
	stand_->Update(vehicle_->GetLocalPosition(), cameraDir);
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	// 必殺弾更新
	for (PlayerSpecialBullet* bullet : specialBullets_) {
		bullet->Update();
	}

	// SP回収処理
	if (vehicle_->GetIsGetShineBall()) {
		nowSPGauge_++;
		if (nowSPGauge_ == kMaxSPGauge_) {
			nowSPGauge_ = 0;
			if (nowSPNum_ < kMaxSPNum_) {
				nowSPNum_++;
			}
		}
		vehicle_->SetIsGetShineBall(false);
	}

	// ダメージ判定
	Damage();
	// 死亡判定
	if (nowHP_ <= 0) {
		isDead_ = true;
	}

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::Text("playerHP : %d/%d", nowHP_, kMaxHP_);
	ImGui::Text("playerSPGauge : %d/%d", nowSPGauge_, kMaxSPGauge_);
	ImGui::Text("playerSPNum : %d/%d", nowSPNum_, kMaxSPNum_);
	ImGui::End();
#endif // _DEBUG
}

void Player::Draw(ViewProjection& viewProjection) {
	// 砲台描画
	stand_->Draw(viewProjection);
	// 車両描画
	vehicle_->Draw(viewProjection);
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	// 必殺弾描画
	for (PlayerSpecialBullet* bullet : specialBullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	// インターバル関連処理
	if (interval_ > 0) {
		interval_--;
	}
	// 通常弾
	if (input_->PushKey(DIK_SPACE)) {

		if (interval_ == 0) {

			// 弾を生成
			PlayerBullet* newBullet = new PlayerBullet();
			// 弾の発射位置を確定
			Vector3 bulletInitPosition = Transform({0.0f, 0.0f, 3.0f}, stand_->GetWorldTransform().matWorld_);
			// 弾の速度(ベクトル)を確定
			Vector3 velocity = Normalize(Subtract(stageScene_->GetReticle()->GetWorldPosition(), bulletInitPosition));
			velocity = Multiply(kBulletSpeed_, velocity);
			// 弾を初期化
			newBullet->Initialize(modelBullet_.get(), bulletInitPosition, velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
			// インターバルリセット
			interval_ = kBulletInterval_;
		}
	}
	// 必殺弾
	if (nowSPNum_ > 0) {
		if (input_->TriggerKey(DIK_B)) {
			// 弾を生成
			PlayerSpecialBullet* newBullet = new PlayerSpecialBullet();
			// 弾の発射位置を確定
			Vector3 bulletInitPosition = Transform({0.0f, 0.0f, 3.0f}, stand_->GetWorldTransform().matWorld_);
			// 弾の速度(ベクトル)を確定
			Vector3 velocity = Normalize(Subtract(stageScene_->GetReticle()->GetWorldPosition(), bulletInitPosition));
			velocity = Multiply(kBulletSpeed_, velocity);
			// 弾を初期化
			newBullet->Initialize(modelSpecialBullet_.get(), bulletInitPosition, velocity);

			// 弾を登録する
			specialBullets_.push_back(newBullet);
			// インターバルリセット
			interval_ = kBulletInterval_;
			//SP消費
			nowSPNum_--;
		}
	}
}

void Player::Damage() {
	if (vehicle_->GetIsDamage()) {
		nowHP_ -= 1;
		vehicle_->SetIsDamage(false);
	}
}
