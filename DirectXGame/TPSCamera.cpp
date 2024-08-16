#include "TPSCamera.h"
#include "Function.h"
#include "StageScene.h"
#include <imgui.h>

void TPSCamera::Initialize(Input* input) {
	// 入力の設定
	input_ = input;
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
}

void TPSCamera::Update() {

	// カメラ切り替え
	if (!isLockOn && !isTransition) {
		// ロックオン状態にする
		if (input_->TriggerKey(DIK_LSHIFT)) {
			// フラグ切り替え
			isLockOn = true;
			isTransition = true;
			// 始めのカメラのワールドポジションを記憶
			setWorldPosition = GetWorldPosition();
		}
	} else if (isLockOn && !isTransition) {
		// ロックオン状態解除
		if (input_->TriggerKey(DIK_LSHIFT)) {
			// フラグ切り替え
			isLockOn = false;
			// 視点移動速度の設定
			speedLat_ = kNormalSpeed_;
			speedLon_ = kNormalSpeed_;
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

void TPSCamera::LockOnCameraProcess() {
	// ロックオン遷移処理
	if (isTransition) {
		// カウントをインクリメント
		countTransition_++;

		// エネミー(chest)→回転中心座標のベクトルを獲得(slerp用に長さを揃える)
		Vector3 enemyToCenterOfRotation = Multiply(length_, Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetChest()->GetWorldPosition())));
		// 回転中心座標→カメラのベクトルを獲得
		Vector3 centerOfRotationToCamera = Multiply(length_, Normalize(Subtract(setWorldPosition, toCenterDirectionWorld_)));
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
		}
	}
	// ロックオン中処理
	else {
		// 敵→回転中心のベクトル
		Vector3 enemyToCenterOfRotation = Normalize(Subtract(toCenterDirectionWorld_, stageScene_->GetEnemy()->GetChest()->GetWorldPosition()));
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
	if (input_->PushKey(DIK_UP)) {
		lat -= speedLat_;
	}
	if (input_->PushKey(DIK_DOWN)) {
		lat += speedLat_;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		lon -= speedLon_;
	}
	if (input_->PushKey(DIK_LEFT)) {
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
