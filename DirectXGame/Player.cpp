#include "Player.h"
#include "ImGuiManager.h"
#include "StageScene.h"
#include <cassert>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, const Vector3& position, Input* input) {
	// 入力
	input_ = input;
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// 砲台モデルの生成
	modelStand_.reset(Model::CreateFromOBJ("playerAbove", true));
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 砲台の生成
	stand_ = std::make_unique<ShootingStand>();
	// 砲台の初期化
	stand_->Initialize(modelStand_.get());
}

void Player::Update() {
	// 位置の更新
	Vector3 velocity;
	velocity = Normalize({stageScene_->GetTPSCamera()->GetDirectionToPlayer().x, 0.0f, stageScene_->GetTPSCamera()->GetDirectionToPlayer().z});
	velocity.x *= speed_;
	velocity.y *= speed_;
	velocity.z *= speed_;
	if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A) && input_->PushKey(DIK_S)&&input_->PushKey(DIK_D)) {
		
	} 
	else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D) && input_->PushKey(DIK_S)) {
		float radian = (2.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_D) && input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
		float radian = (4.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A) && input_->PushKey(DIK_W)) {
		float radian = (6.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_A) && input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
		float radian = (0.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
		float radian = (1.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	}
	else if (input_->PushKey(DIK_D) && input_->PushKey(DIK_S)) {
		float radian = (3.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	}
	else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
		float radian = (5.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_A) && input_->PushKey(DIK_W)) {
		float radian = (7.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_W)) {
		float radian = (0.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_D)) {
		float radian = (2.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	} 
	else if (input_->PushKey(DIK_S)) {
		float radian = (4.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	}
	else if (input_->PushKey(DIK_A)) {
		float radian = (6.0f / 4.0f) * pi;
		velocity = Transform(velocity, MakeRotateYMatrix(radian));
		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;
	}


	// 回転の更新
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(velocity, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 砲台の更新
	stand_->Update(worldTransform_.translation_);
	// 行列の再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

#ifdef _DEBUG
	ImGui::Begin("debugPlayer");

	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::End();
#endif // _DEBUG
}

void Player::Draw(ViewProjection& viewProjection) {
	// 砲台描画
	stand_->Draw(viewProjection);
	// プレイヤー本体描画
	model_->Draw(worldTransform_, viewProjection);
}
