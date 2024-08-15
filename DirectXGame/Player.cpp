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
	// 車両の生成
	vehicle_ = std::make_unique<Vehicle>();
	//車両にstageSceneをセット
	vehicle_->SetStageScene(stageScene_);
	// 車両の初期化
	vehicle_->Initialize(input_, modelVehicle_.get(), position);
	// 砲台の生成
	stand_ = std::make_unique<ShootingStand>();
	// 砲台の初期化
	stand_->Initialize(modelStand_.get());


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

	//攻撃処理
	Attack();

	// 車両の更新
	vehicle_->Update();
	// 砲台の更新
	Vector3 cameraDir = {stageScene_->GetTPSCamera()->GetDirectionToPlayer().x, 0.0f, stageScene_->GetTPSCamera()->GetDirectionToPlayer().z};
	stand_->Update(vehicle_->GetLocalPosition(), cameraDir);
	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

#ifdef _DEBUG

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
		
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾を生成
		PlayerBullet* newBullet = new PlayerBullet();
		// 弾の発射位置を確定
		Vector3 bulletInitPosition = Transform({0.0f,0.0f,3.0f},stand_->GetWorldTransform().matWorld_);
		//弾の速度(ベクトル)を確定
		Vector3 velocity = Normalize(Subtract(stageScene_->GetReticle()->GetWorldPosition(), bulletInitPosition));
		velocity = Multiply(kBulletSpeed_, velocity);
		//弾を初期化
		newBullet->Initialize(modelBullet_.get(), bulletInitPosition, velocity);
		

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

