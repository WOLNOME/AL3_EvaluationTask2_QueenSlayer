#include "TPSCamera.h"
#include "Function.h"
#include "ImGuiManager.h"
#include "StageScene.h"
#include "TextureManager.h"
#include "WinApp.h"

void TPSCamera::Initialize(Input* input, GamePad* pad) {
	// 入力の設定
	input_ = input;
	pad_ = pad;
	// ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.x = toCenterDirectionLocal_.x + length_ * cosf(lat) * cosf(lon);
	worldTransform_.translation_.y = toCenterDirectionLocal_.y + length_ * sinf(lat);
	worldTransform_.translation_.z = toCenterDirectionLocal_.z + length_ * cosf(lat) * sinf(lon);
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	// directionの初期化
	direction_ = Normalize(Subtract(toCenterDirectionLocal_, worldTransform_.translation_));

	// ロックオンスプライトのテクスチャを読み込む
	textureHandleLockOnTarget_ = TextureManager::Load("lockOnTarget.png");
	// 2Dスプライト
	spriteLockOnTarget_.reset(Sprite::Create(textureHandleLockOnTarget_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	// 2Dスプライトの元のサイズ取得
	lockOnTargetSize_ = spriteLockOnTarget_->GetSize();
}

void TPSCamera::Update() {

	// カメラ切り替え
	if (!isLockOn && !isTransition) {
		// ロックオン状態にする
		if (input_->TriggerKey(DIK_LSHIFT) || pad_->TriggerLB()) {
			// フラグ切り替え
			isLockOn = true;
			isTransition = true;
			// ロックオンパーツの指定
			lockOnParts = CHEST;
		}
	} else if (isLockOn && !isTransition) {
		// ロックオン状態解除
		if (input_->TriggerKey(DIK_LSHIFT) || pad_->TriggerLB()) {
			// フラグ切り替え
			isLockOn = false;
			/// 今のカメラの座標をlatとlonに教える
			// oLatとoLonの座標を得る
			Vector3 sphereOrigin;
			sphereOrigin.x = toCenterDirectionLocal_.x + length_ * cosf(0.0f) * cosf(0.0f);
			sphereOrigin.y = toCenterDirectionLocal_.y + length_ * sinf(0.0f);
			sphereOrigin.z = toCenterDirectionLocal_.z + length_ * cosf(0.0f) * sinf(0.0f);
			// 回転中心(ローカル)→球オリジン座標のベクトルを得る
			Vector3 centerOfRotationToOrigin = Normalize(Subtract(sphereOrigin, toCenterDirectionLocal_));
			// 回転中心(ローカル)→現在のカメラの座標(ローカル)
			Vector3 centerOfRotationToCamera = Normalize(Subtract(worldTransform_.translation_, toCenterDirectionLocal_));
			// lat用ベクトルの生成
			Vector3 centerOfRotationToCameraLat = Normalize(Vector3(1.0f, centerOfRotationToCamera.y, 0.0f));
			// 2つのベクトルの回転量をlatとlonに代入
			lat = -AngleOf2VectorZ(centerOfRotationToOrigin, centerOfRotationToCameraLat);
			lon = -AngleOf2VectorY(centerOfRotationToOrigin, centerOfRotationToCamera);
		}
	}
	// 回転中心座標(world)の更新
	toCenterDirectionWorld_ = Add(stageScene_->GetPlayer()->GetWorldPostion(), toCenterDirectionLocal_);
	// 各カメラ処理
	if (isLockOn) { // ロックオンカメラ処理
		LockOnCameraProcess();
	} else { // ノーマルカメラ処理
		NormalCameraProcess();
	}

	// 行列の再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// プレイヤーの座標だけを掛ける(プレイヤーの座標を基準にする(回転は引き継がない))
	worldTransform_.matWorld_ = Multiply(worldTransform_.matWorld_, MakeTranslateMatrix(stageScene_->GetPlayer()->GetWorldPostion()));
	// 行列の転送
	worldTransform_.TransferMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

#ifdef _DEBUG
	ImGui::Begin("debugTPSCamera");
	ImGui::Text("lat(y):%f,lon(x):%f", lat, lon);
	ImGui::End();

#endif // _DEBUG
}

void TPSCamera::DrawUI() {
	// ロックオン遷移状態か、ロックオン状態の時のみ
	if (isTransition || isLockOn) {
		spriteLockOnTarget_->Draw();
	}
}

void TPSCamera::LockOnCameraProcess() {
	// ロックオン遷移処理
	if (isTransition) {
		// カウントをインクリメント
		countTransition_++;

		// ロックオンUIのサイズを小さくする
		Vector2 newLockOnTargetSize;
		newLockOnTargetSize.x = lockOnTargetSize_.x - (lockOnTargetSize_.x - (lockOnTargetSize_.x * kLockOnTargetGoalSize_)) * ((float)countTransition_ / kTransitionFrame);
		newLockOnTargetSize.y = lockOnTargetSize_.y - (lockOnTargetSize_.y - (lockOnTargetSize_.y * kLockOnTargetGoalSize_)) * ((float)countTransition_ / kTransitionFrame);
		spriteLockOnTarget_->SetSize(newLockOnTargetSize);

		// エネミー→回転中心座標のベクトルを獲得(slerp用に長さを揃える)
		Vector3 enemyToCenterOfRotation;
		// 各部位で変数を目標を変更
		switch (lockOnParts) {
		case HEAD:
			enemyToCenterOfRotation = Multiply(length_, Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetHead()->GetWorldPosition())));
			break;
		case CHEST:
			enemyToCenterOfRotation = Multiply(length_, Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetChest()->GetWorldPosition())));
			break;
		case STOMACH:
			enemyToCenterOfRotation = Multiply(length_, Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetStomach()->GetWorldPosition())));
			break;
		default:
			break;
		}
		// 回転中心座標→カメラのベクトルを獲得
		Vector3 centerOfRotationToCamera = Multiply(length_, Normalize(Subtract(GetWorldPosition(), toCenterDirectionWorld_)));
		// スラープ関数で新しい座標を得る
		Vector3 slerpNewPosition = Multiply(length_, Normalize(Slerp(centerOfRotationToCamera, enemyToCenterOfRotation, (float)countTransition_ / kTransitionFrame)));

		// 座標の更新(rotateXYを回転中心ローカル座標に加算)
		worldTransform_.translation_.x = toCenterDirectionLocal_.x + slerpNewPosition.x;
		worldTransform_.translation_.y = toCenterDirectionLocal_.y + slerpNewPosition.y;
		worldTransform_.translation_.z = toCenterDirectionLocal_.z + slerpNewPosition.z;
		// 回転の加算(座標を基に)
		direction_ = Normalize(Subtract(toCenterDirectionLocal_, worldTransform_.translation_));
		worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
		Vector3 velocityZ = Transform(direction_, rotateYMatrix);
		worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

		// カウントが0になった時
		if (countTransition_ == kTransitionFrame) {
			// フラグ管理
			isTransition = false;
			// カウントリセット
			countTransition_ = 0;
			// ロックオンUIサイズセット
			spriteLockOnTarget_->SetSize({lockOnTargetSize_.x * kLockOnTargetGoalSize_, lockOnTargetSize_.y * kLockOnTargetGoalSize_});
		}
	}
	// ロックオン中処理
	else {
		// ロックオン切り替え処理
		switch (lockOnParts) {
		case HEAD:
			// ロックオン部位の切り替え
			if (input_->TriggerKey(DIK_DOWN) || pad_->TriggerRStickDOWN()) {
				// フラグ切り替え
				isLockOn = true;
				isTransition = true;
				// ロックオンパーツの指定
				lockOnParts = CHEST;
			}
			break;
		case CHEST:
			// ロックオン部位の切り替え
			if (input_->TriggerKey(DIK_UP) || pad_->TriggerRStickUP()) {
				// フラグ切り替え
				isLockOn = true;
				isTransition = true;
				// ロックオンパーツの指定
				lockOnParts = HEAD;
			}
			if (input_->TriggerKey(DIK_DOWN) || pad_->TriggerRStickDOWN()) {
				// フラグ切り替え
				isLockOn = true;
				isTransition = true;
				// ロックオンパーツの指定
				lockOnParts = STOMACH;
			}
			break;
		case STOMACH:
			// ロックオン部位の切り替え
			if (input_->TriggerKey(DIK_UP) || pad_->TriggerRStickUP()) {
				// フラグ切り替え
				isLockOn = true;
				isTransition = true;
				// ロックオンパーツの指定
				lockOnParts = CHEST;
			}
			break;
		default:
			break;
		}

		// 敵→回転中心のベクトル
		Vector3 enemyToCenterOfRotation;
		// 各部位で変数を目標を変更
		switch (lockOnParts) {
		case HEAD:
			enemyToCenterOfRotation = Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetHead()->GetWorldPosition()));
			break;
		case CHEST:
			enemyToCenterOfRotation = Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetChest()->GetWorldPosition()));
			break;
		case STOMACH:
			enemyToCenterOfRotation = Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetStomach()->GetWorldPosition()));
			break;
		default:
			break;
		}
		// 手に入れたベクトルに距離を掛ける
		Vector3 centerOfRotationToCamera = Multiply(length_, enemyToCenterOfRotation);

		// 座標の更新(上記のベクトルを回転中心ローカル座標に加算した値)
		worldTransform_.translation_.x = toCenterDirectionLocal_.x + centerOfRotationToCamera.x;
		worldTransform_.translation_.y = toCenterDirectionLocal_.y + centerOfRotationToCamera.y;
		worldTransform_.translation_.z = toCenterDirectionLocal_.z + centerOfRotationToCamera.z;
		// 回転の加算(座標を基に)
		direction_ = Normalize(Subtract(toCenterDirectionLocal_, worldTransform_.translation_));
		worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
		Vector3 velocityZ = Transform(direction_, rotateYMatrix);
		worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	}
}

void TPSCamera::NormalCameraProcess() {
	// 移動の加算
	if ((input_->PushKey(DIK_UP) && input_->PushKey(DIK_RIGHT) && input_->PushKey(DIK_DOWN) && input_->PushKey(DIK_LEFT))) {
	} else if ((input_->PushKey(DIK_UP) && input_->PushKey(DIK_RIGHT) && input_->PushKey(DIK_DOWN))) {
		lon -= speedLon_;
	} else if ((input_->PushKey(DIK_RIGHT) && input_->PushKey(DIK_DOWN) && input_->PushKey(DIK_LEFT))) {
		lat += speedLat_;
	} else if ((input_->PushKey(DIK_UP) && input_->PushKey(DIK_DOWN) && input_->PushKey(DIK_LEFT))) {
		lon += speedLon_;
	} else if ((input_->PushKey(DIK_UP) && input_->PushKey(DIK_RIGHT) && input_->PushKey(DIK_LEFT))) {
		lat -= speedLat_;
	} else if ((input_->PushKey(DIK_UP) && input_->PushKey(DIK_RIGHT)) || pad_->TiltRStickUPRIGHT()) {
		lat -= speedLat_;
		lon -= speedLon_;
	} else if ((input_->PushKey(DIK_RIGHT) && input_->PushKey(DIK_DOWN)) || pad_->TiltRStickDOWNRIGHT()) {
		lat += speedLat_;
		lon -= speedLon_;
	} else if ((input_->PushKey(DIK_DOWN) && input_->PushKey(DIK_LEFT)) || pad_->TiltRStickDOWNLEFT()) {
		lat += speedLat_;
		lon += speedLon_;
	} else if ((input_->PushKey(DIK_LEFT) && input_->PushKey(DIK_UP)) || pad_->TiltRStickUPLEFT()) {
		lat -= speedLat_;
		lon += speedLon_;
	} else if (input_->PushKey(DIK_UP) || pad_->TiltRStickUP()) {
		lat -= speedLat_;
	} else if (input_->PushKey(DIK_DOWN) || pad_->TiltRStickDOWN()) {
		lat += speedLat_;
	} else if (input_->PushKey(DIK_RIGHT) || pad_->TiltRStickRIGHT()) {
		lon -= speedLon_;
	} else if (input_->PushKey(DIK_LEFT) || pad_->TiltRStickLEFT()) {
		lon += speedLon_;
	}
	// 移動制限
	lat = max(lat, -1.0f / 10.0f * pi);
	lat = min(lat, 1.0f / 15.0f * pi);
	// 座標の更新
	worldTransform_.translation_.x = toCenterDirectionLocal_.x + length_ * cosf(lat) * cosf(lon);
	worldTransform_.translation_.y = toCenterDirectionLocal_.y + length_ * sinf(lat);
	worldTransform_.translation_.z = toCenterDirectionLocal_.z + length_ * cosf(lat) * sinf(lon);

	// 回転の加算(座標を基に)
	direction_ = Normalize(Subtract(toCenterDirectionLocal_, worldTransform_.translation_));
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
}

const Vector3 TPSCamera::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
