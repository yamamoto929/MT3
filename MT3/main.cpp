#include <Novice.h>
#include <imgui.h>
#include <cmath>
#include "Functions.h"
#include "Matrix4x4.h"
#include "Matrix4x4Function.h"
#include "Sphere.h"

const char kWindowTitle[] = "LE2B_30_ヤマモト_ルナ_MT3_02_00";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Sphere sphere[2]{};
	sphere[0].radius = 1.0f;
	sphere[0].center = Vector3{ 0.0f,1.0f,0.0f };

	sphere[1].radius = 1.0f;
	sphere[1].center = Vector3{ 2.0f,1.0f,0.0f };
	int mouseX = 0;
	int mouseY = 0;

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

		if (Novice::IsPressMouse(1)) {
			int curMouseX;
			int curMouseY;

			Novice::GetMousePosition(&curMouseX, &curMouseY);
			cameraRotate.x += float(mouseY - curMouseY)/1000.0f;
			cameraRotate.y += float(mouseX - curMouseX)/1000.0f;
			mouseX = curMouseX;
			mouseY = curMouseY;
		} else {
			Novice::GetMousePosition(&mouseX, &mouseY);
		}

		float speed = 0.1f; 

		float yaw = cameraRotate.y;

		
		Vector3 forward = { std::sin(yaw), 0.0f, std::cos(yaw) };
		
		Vector3 right = { std::cos(yaw), 0.0f, -std::sin(yaw) };

		if (keys[DIK_W]) {
			cameraTranslate.x += forward.x * speed;
			cameraTranslate.z += forward.z * speed;
		} else if (keys[DIK_S]) {
			cameraTranslate.x -= forward.x * speed;
			cameraTranslate.z -= forward.z * speed;
		}

		if (keys[DIK_A]) {
			cameraTranslate.x -= right.x * speed;
			cameraTranslate.z -= right.z * speed;
		} else if (keys[DIK_D]) {
			cameraTranslate.x += right.x * speed;
			cameraTranslate.z += right.z * speed;
		}

		if (keys[DIK_SPACE]) {
			cameraTranslate.y += speed;
		} else if (keys[DIK_LSHIFT]) {
			cameraTranslate.y -= speed;
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

		DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		if (IsCollision(sphere[0], sphere[1])) {
			DrawSphere(sphere[1], viewProjectionMatrix, viewportMatrix, 0xFF0000FF);
		} else {
			DrawSphere(sphere[1], viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		}
		


		ImGui::Begin("Window");
		ImGui::InputFloat3("sphere[0].center",
			&sphere[0].center.x,
			"%.3f");
		ImGui::InputFloat("sphere[0].radius",
			&sphere[0].radius);

		ImGui::InputFloat3("sphere[1].center",
			&sphere[1].center.x,
			"%.3f");
		ImGui::InputFloat("sphere[1].radius",
			&sphere[1].radius);
		ImGui::End();
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
