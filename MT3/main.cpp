#include <Novice.h>
#include <cmath>
#include <imgui.h>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Draw.h"
#include "ConicalPedulum.h"

const int kWindowWidth = 1280;
const int kWindowHeight = 720;
const char kWindowTitle[] = "LE2B_30_ヤマモト_ルナ_MT3_04_03";
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float deltaTime = 1.0f / 60.0f;

	bool isActive = false;

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	ConicalPendulum initConicalPendulum;
	initConicalPendulum.anchor = { 0.0f, 1.0f, 0.0f };
	initConicalPendulum.length = 0.8f;
	initConicalPendulum.halfApexAngle = 0.7f;
	initConicalPendulum.angle = 0.0f;
	initConicalPendulum.angularVelocity = 0.0f;

	ConicalPendulum conicalPendulum = initConicalPendulum;

	Sphere sphere;
	sphere.radius = 0.05f;
	sphere.center = {};

	Vector3 centerPos = {};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Window");
		if (ImGui::Button("Start")) {
			isActive = true;
		}

		if (ImGui::Button("Reset")) {
			conicalPendulum = initConicalPendulum;
		}

		ImGui::SliderFloat("Length", &conicalPendulum.length, 0.1f, 2.0f);
		ImGui::SliderFloat("HalfApexAngle", &conicalPendulum.halfApexAngle, 0.1f, f);
		ImGui::End();

		if(isActive){
			conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
			float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			sphere.center.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
			sphere.center.y = conicalPendulum.anchor.y - height;
			sphere.center.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;
		}

		Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3{ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawSegment(conicalPendulum.anchor, sphere.center, viewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}


