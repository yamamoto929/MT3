#include <Novice.h>
#include <imgui.h>
#include <cmath>
#include "Functions.h"
#include "Matrix4x4.h"
#include "AABB.h"
#include <algorithm>

const char kWindowTitle[] = "LE2B_30_ヤマモト_ルナ_MT3_02_05";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;
void MoveCamera(char keys[], Vector3& rotate, Vector3& translate);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	AABB aabb1{
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.0f,0.0f,0.0f}
	};

	AABB aabb2{
		.min{0.2f,0.2f,0.2f},
		.max{ 1.0f,1.0f,1.0f}
	};

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

		MoveCamera(keys, cameraRotate, cameraTranslate);

		ImGui::Begin("Debug");

		ImGui::DragFloat3("aabb1.min",
			&aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max",
			&aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb2.min",
			&aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max",
			&aabb2.max.x, 0.01f);
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);

		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);
		ImGui::End();

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
		DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		if (IsCollision(aabb1, aabb2)) {
			DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, 0xFF0000FF);
		} else {
			DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		}

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

void MoveCamera(char keys[], Vector3& rotate, Vector3& translate) {

	static int mouseX = 0;
	static int mouseY = 0;
	if (Novice::IsPressMouse(1)) {
		int curMouseX;
		int curMouseY;


		Novice::GetMousePosition(&curMouseX, &curMouseY);
		rotate.x += float(mouseY - curMouseY) / 1000.0f;
		rotate.y += float(mouseX - curMouseX) / 1000.0f;
		mouseX = curMouseX;
		mouseY = curMouseY;
	} else {
		Novice::GetMousePosition(&mouseX, &mouseY);
	}

	float speed = 0.1f;

	float yaw = rotate.y;


	Vector3 forward = { std::sin(yaw), 0.0f, std::cos(yaw) };

	Vector3 right = { std::cos(yaw), 0.0f, -std::sin(yaw) };

	if (keys[DIK_W]) {
		translate.x += forward.x * speed;
		translate.z += forward.z * speed;
	} else if (keys[DIK_S]) {
		translate.x -= forward.x * speed;
		translate.z -= forward.z * speed;
	}

	if (keys[DIK_A]) {
		translate.x -= right.x * speed;
		translate.z -= right.z * speed;
	} else if (keys[DIK_D]) {
		translate.x += right.x * speed;
		translate.z += right.z * speed;
	}

	if (keys[DIK_SPACE]) {
		translate.y += speed;
	} else if (keys[DIK_LSHIFT]) {
		translate.y -= speed;
	}
};
