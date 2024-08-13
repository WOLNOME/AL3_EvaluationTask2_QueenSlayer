#include "GameScene.h"
#include "AxisIndicator.h"
#include "BaseScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "StageScene.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// シーンの生成
	m_pScene = std::make_unique<TitleScene>();
	//シーンの初期化
	m_pScene->Init(input_);
	CurrentScene_ = TITLE;
	NextScene_ = TITLE;
}

void GameScene::Update() {
	// シーンの更新処理
	m_pScene->Update();
	// 各シーンからNextSceneの情報を受け取る
	if (NextScene_ != m_pScene->GetNextScene()) {
		NextScene_ = m_pScene->GetNextScene();
	}
	// シーンの切り替え処理
	ChangeScene();


	
#ifdef _DEBUG
	ImGui::Begin("debug");
	int nowScene = CurrentScene_;
	ImGui::Text("nowScene:%d", nowScene);

	ImGui::End();
#endif // _DEBUG

}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// シーンの描画処理
	m_pScene->Draw(commandList, dxCommon_);


}

void GameScene::ChangeScene() {
	// シーン切り替えアニメーションとか作るならこの関数に書く

	// 切り替え
	if (NextScene_ != CurrentScene_) {
		// 現在のシーンの削除
		if (m_pScene != NULL) {
			m_pScene.reset();
		}
		// 次のシーンの挿入
		switch (NextScene_) { // 引数のシーン
		case SCENE::TITLE:
			m_pScene = std::make_unique<TitleScene>(); // タイトルシーンを現在のシーンにする
			m_pScene->Init(input_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;
		case SCENE::STAGE:
			m_pScene = std::make_unique<StageScene>(); // タイトルシーンを現在のシーンにする
			m_pScene->Init(input_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;
		default:
			break;
		}
	}
}
