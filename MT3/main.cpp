#include <Novice.h>
#include <imgui.h>
#include <cmath>
#include "Functions.h"
#include "OBB.h"
#include "Sphere.h"
#include "Matrix4x4.h"
#include <algorithm>

const char kWindowTitle[] = "LE2B_30_ヤマモト_ルナ_MT3_02_09_EX";
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

	Vector3 rotate1{ 0.0f,0.0f,0.0f };
	Vector3 rotate2{ -0.05f,-2.49f,0.15f };
	OBB obb1{
		.center{-1.0f,0.0f,0.0f},
		.orientations = { {1.0f,0.0f,0.0f},
						{0.0f,1.0f,0.0f},
						{0.0f,0.0f,1.0f}},
		.size{0.83f,0.26f,0.24f}
	};

	OBB obb2{
		.center{0.9f,0.66f,0.78f},
		.orientations = { {1.0f,0.0f,0.0f},
						{0.0f,1.0f,0.0f},
						{0.0f,0.0f,1.0f}},
		.size{0.5f,0.37f,0.5f}
	};

	
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

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
		//==========================================================
		// OBB1
		//==========================================================
		ImGui::DragFloat3("obb1.center",
			&obb1.center.x, 0.01f);
		ImGui::SliderAngle("rotate1.x",
			&rotate1.x);
		ImGui::SliderAngle("rotate1.y",
			&rotate1.y);
		ImGui::SliderAngle("rotate1.z",
			&rotate1.z);
		MakeOBBOrientation(rotate1, obb1);
		ImGui::DragFloat3("obb1.orientations[0]",
			&obb1.orientations[0].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::DragFloat3("obb1.orientations[1]",
			&obb1.orientations[1].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::DragFloat3("obb1.orientations[2]",
			&obb1.orientations[2].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);

		ImGui::DragFloat3("obb1.size",
			&obb1.size.x, 0.01f, 0.01f, 2.0f);

		//==========================================================
		// OBB2
		//==========================================================
		ImGui::DragFloat3("obb2.center",
			&obb2.center.x, 0.01f);
		ImGui::SliderAngle("rotate2.x",
			&rotate2.x);
		ImGui::SliderAngle("rotate2.y",
			&rotate2.y);
		ImGui::SliderAngle("rotate2.z",
			&rotate2.z);
		MakeOBBOrientation(rotate2, obb2);
		ImGui::DragFloat3("obb2.orientations[0]",
			&obb2.orientations[0].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::DragFloat3("obb2.orientations[1]",
			&obb2.orientations[1].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::DragFloat3("obb2.orientations[2]",
			&obb2.orientations[2].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);

		ImGui::DragFloat3("obb2.size",
			&obb2.size.x, 0.01f, 0.01f, 2.0f);
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
		DrawOBB(obb2, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		
		if (IsCollision(obb1,obb2)) {
			DrawOBB(obb1, viewProjectionMatrix, viewportMatrix, 0xFF0000FF);
		} else {
			DrawOBB(obb1, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
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
