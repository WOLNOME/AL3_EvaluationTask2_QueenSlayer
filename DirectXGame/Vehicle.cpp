#include "Vehicle.h"
#include "ImGuiManager.h"
#include "StageScene.h"
#include "CollisionConfig.h"
#include "TextureManager.h"
#include <cassert>

Vehicle::Vehicle() {}

Vehicle::~Vehicle() {}

void Vehicle::Initialize(Input* input, Model* model, const Vector3& position,UseScene useScene) {
	//使用されるシーン設定
	useScene_ = useScene;

	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// 入力
		input_ = input;
		// NULLポインタチェック
		assert(model);
		model_ = model;
		// ワールドトランスフォーム設定
		worldTransform_.Initialize();
		// 引数で受け取った座標をセット
		worldTransform_.translation_ = position;
		// 衝突属性を設定(自分の属性)
		SetCollisionAttribute(kCollisionAttributePlayer);
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		// NULLポインタチェック
		assert(model);
		model_ = model;
		// テクスチャハンドル
		textureHandle_ = TextureManager::Load("player/player_tank_damage.png");
		// ワールドトランスフォーム設定
		worldTransform_.Initialize();
		// 引数で受け取った座標をセット
		worldTransform_.translation_ = position;

		break;
	default:
		break;
	}
	
}

void Vehicle::Update() {
	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// 向きベクトルの初期化
		if (direction_.y > 0.0f) {
			direction_ = Normalize({stageScene_->GetTPSCamera()->GetDirectionToPlayer().x, 0.0f, stageScene_->GetTPSCamera()->GetDirectionToPlayer().z});
		}

		// 位置の更新
		cameraDir = {stageScene_->GetTPSCamera()->GetDirectionToPlayer().x, 0.0f, stageScene_->GetTPSCamera()->GetDirectionToPlayer().z};
		// 入力したベクトル
		Vector3 inputVector = Normalize(cameraDir);
		// 入力処理
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A) && input_->PushKey(DIK_S) && input_->PushKey(DIK_D)) {

		} else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D) && input_->PushKey(DIK_S)) {
			radian_ = (2.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_D) && input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
			radian_ = (4.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A) && input_->PushKey(DIK_W)) {
			radian_ = (6.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_A) && input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
			radian_ = (0.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
			radian_ = (1.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_D) && input_->PushKey(DIK_S)) {
			radian_ = (3.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
			radian_ = (5.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_A) && input_->PushKey(DIK_W)) {
			radian_ = (7.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_W)) {
			radian_ = (0.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_D)) {
			radian_ = (2.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_S)) {
			radian_ = (4.0f / 4.0f) * pi;
			isMove = true;
		} else if (input_->PushKey(DIK_A)) {
			radian_ = (6.0f / 4.0f) * pi;
			isMove = true;
		}
		// 移動処理
		if (isMove) {
			// 入力Vector獲得
			inputVector = Transform(inputVector, MakeRotateYMatrix(radian_));
			// directionとvelocityのなす角を求める
			float theta = AngleOf2VectorY(direction_, inputVector); // ←direction基準
			// 最終的に加算する速度
			Vector3 velocity = {0.0f, 0.0f, 0.0f};
			// 前進
			if (theta == 0.0f) {
				// そのままのdirection方向に進む
				velocity = Multiply(speed_, Normalize(direction_));
				// directionはvelocity方向
				direction_ = velocity;
			}
			// 後退
			else if (theta == pi || theta == -pi) {
				// そのままのdirectionの逆方向に進む
				velocity = Multiply(speed_, Normalize(direction_));
				velocity = Multiply(-1.0f, velocity);
				// directionはvelocityの逆方向
				direction_ = Multiply(-1.0f, velocity);
			}
			// 左折
			else if (theta >= -(1.0f / 2.0f) * pi && theta < 0.0f) {
				// θが回転速度より大きかったら
				if (std::abs(theta) >= std::abs(rotateSpeed_)) {
					// 速度はdirectionをマイナス回転させた方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(-rotateSpeed_)));
				}
				// θが回転速度より小さかったら
				else {
					// 速度はdirectionをθ回転させた方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(theta)));
				}
				// directionはvelocity方向
				direction_ = velocity;
			}
			// 右折
			else if (theta <= (1.0f / 2.0f) * pi && theta > 0.0f) {
				// θが回転速度より大きかったら
				if (std::abs(theta) >= std::abs(rotateSpeed_)) {
					// 速度はdirectionをプラス回転させた方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(rotateSpeed_)));
				}
				// θが回転速度より小さかったら
				else {
					// 速度はdirectionをθ回転させた方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(theta)));
				}
				// directionはvelocity方向
				direction_ = velocity;
			}
			// 左バック
			else if (theta < -(1.0f / 2.0f) * pi && theta > -pi) {
				// θ2の定義(dirと-inputのなす角)
				float theta2 = pi + theta;
				// θ2が回転速度より大きかったら
				if (std::abs(theta2) >= std::abs(rotateSpeed_)) {
					// 速度はdirectionをプラス回転させた方向の逆方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(rotateSpeed_)));
					velocity = Multiply(-1.0f, velocity);
				}
				// θ2が回転速度より小さかったら
				else {
					// 速度はdirectionをθ2回転させた方向の逆方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(theta2)));
					velocity = Multiply(-1.0f, velocity);
				}
				// directionはvelocityと逆方向
				direction_ = Multiply(-1.0f, velocity);
			}
			// 右バック
			else if (theta > (1.0f / 2.0f) * pi && theta < pi) {
				// θ2の定義(dirと-inputのなす角)
				float theta2 = -pi + theta;
				// θ2が回転速度より大きかったら
				if (std::abs(theta2) >= std::abs(rotateSpeed_)) {
					// 速度はdirectionをマイナス回転させた方向の逆方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(-rotateSpeed_)));
					velocity = Multiply(-1.0f, velocity);
				}
				// θ2が回転速度より小さかったら
				else {
					// 速度はdirectionをθ2回転させた方向の逆方向
					velocity = Multiply(speed_, Transform(Normalize(direction_), MakeRotateYMatrix(theta2)));
					velocity = Multiply(-1.0f, velocity);
				}
				// directionはvelocityと逆方向
				direction_ = Multiply(-1.0f, velocity);
			}

			// 移動の更新
			worldTransform_.translation_.x += velocity.x;
			worldTransform_.translation_.y += velocity.y;
			worldTransform_.translation_.z += velocity.z;

			// 移動の制限
			worldTransform_.translation_.x = max(worldTransform_.translation_.x, -60.0f);
			worldTransform_.translation_.x = min(worldTransform_.translation_.x, 60.0f);
			worldTransform_.translation_.z = max(worldTransform_.translation_.z, -60.0f);
			worldTransform_.translation_.z = min(worldTransform_.translation_.z, 60.0f);

			// 回転の更新
			worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
			Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
			Vector3 velocityZ = Transform(direction_, rotateYMatrix);
			worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

			// フラグの完了
			isMove = false;
		}
		// ローカル座標
		localPos_ = worldTransform_.translation_;
		// 行列の再計算と転送
		worldTransform_.UpdateMatrix();
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		// 行列の再計算と転送
		worldTransform_.UpdateMatrix();
		break;
	default:
		break;
	}

	
}

void Vehicle::Draw(const ViewProjection& viewProjection) {
	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// 車両本体描画
		model_->Draw(worldTransform_, viewProjection);
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		// 車両本体描画
		model_->Draw(worldTransform_, viewProjection,textureHandle_);
		break;
	default:
		break;
	}
	
}

void Vehicle::OnCollision() { isDamage_ = true; }

void Vehicle::OnSpecialCollision() { isGetShineBall_ = true; }

Vector3 Vehicle::GetWorldPosition() { 
	// ワールド座標
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
