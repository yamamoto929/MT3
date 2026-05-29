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

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	OBB obb{
		.center{-1.0f,0.0f,0.0f},
		.orientations = { {1.0f,0.0f,0.0f},
						{0.0f,1.0f,0.0f},
						{0.0f,1.0f,0.0f}},
		.size{0.5f,0.5f,0.5f}
	};

	Segment segment{
		.origin{-0.8f, -0.3f, 0.0f},
		.diff{0.5f, 0.5f, 0.5f}
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

		ImGui::DragFloat3("obb.center",
			&obb.center.x, 0.01f);
		ImGui::SliderAngle("rotate.x",
			&rotate.x);
		ImGui::SliderAngle("rotate.y",
			&rotate.y);
		ImGui::SliderAngle("rotate.z",
			&rotate.z);
		MakeOBBOrientation(rotate, obb);
		ImGui::DragFloat3("obb.orientations[0]",
			&obb.orientations[0].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::DragFloat3("obb.orientations[1]",
			&obb.orientations[1].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::DragFloat3("obb.orientations[2]",
			&obb.orientations[2].x, 0.01f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);

		ImGui::DragFloat3("obb.size",
			&obb.size.x, 0.01f, 0.01f, 2.0f);

		ImGui::DragFloat3("segment.origin",
			&segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment.diff",
			&segment.diff.x, 0.01f);
		ImGui::End();


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
		DrawSegment(segment, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		if (IsCollision(obb,segment)) {
			DrawOBB(obb, viewProjectionMatrix, viewportMatrix, 0xFF0000FF);
		} else {
			DrawOBB(obb, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
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
